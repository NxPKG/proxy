#pragma once

#include "envoy/json/json_object.h"
#include "envoy/network/filter.h"
#include "envoy/server/filter_config.h"

#include "source/common/common/logger.h"

#include "khulnasoft/api/bpf_metadata.pb.h"
#include "khulnasoft/conntrack.h"
#include "khulnasoft/host_map.h"
#include "khulnasoft/ipcache.h"
#include "khulnasoft/network_policy.h"
#include "khulnasoft/socket_option.h"

namespace Envoy {
namespace Khulnasoft {
namespace BpfMetadata {

/**
 * Global configuration for Bpf Metadata listener filter. This
 * represents all global state shared among the working thread
 * instances of the filter.
 */
class Config : public Khulnasoft::PolicyResolver,
               public std::enable_shared_from_this<Config>,
               Logger::Loggable<Logger::Id::config> {
public:
  Config(const ::khulnasoft::BpfMetadata& config,
         Server::Configuration::ListenerFactoryContext& context);
  virtual ~Config() {}

  // PolicyResolver
  uint32_t resolvePolicyId(const Network::Address::Ip*) const override;
  const PolicyInstanceConstSharedPtr getPolicy(const std::string&) const override;

  virtual bool getMetadata(Network::ConnectionSocket& socket);

  uint32_t proxy_id_;
  bool is_ingress_;
  bool use_original_source_address_;
  bool is_l7lb_;
  Network::Address::InstanceConstSharedPtr ipv4_source_address_;
  Network::Address::InstanceConstSharedPtr ipv6_source_address_;
  bool enforce_policy_on_l7lb_;

  std::shared_ptr<const Khulnasoft::NetworkPolicyMap> npmap_{};
  Khulnasoft::CtMapSharedPtr ct_maps_{};
  Khulnasoft::IPCacheSharedPtr ipcache_{};
  std::shared_ptr<const Khulnasoft::PolicyHostMap> hosts_{};
};

typedef std::shared_ptr<Config> ConfigSharedPtr;

/**
 * Implementation of a bpf metadata listener filter.
 */
class Instance : public Network::ListenerFilter, Logger::Loggable<Logger::Id::filter> {
public:
  Instance(const ConfigSharedPtr& config) : config_(config) {}

  // Network::ListenerFilter
  Network::FilterStatus onAccept(Network::ListenerFilterCallbacks& cb) override;

  // Network::ListenerFilter
  Network::FilterStatus onData(Network::ListenerFilterBuffer& buffer) override;

  // Network::ListenerFilter
  size_t maxReadBytes() const override;

private:
  const ConfigSharedPtr config_;
};

} // namespace BpfMetadata
} // namespace Khulnasoft
} // namespace Envoy
