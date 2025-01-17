#pragma once

#include "envoy/config/grpc_mux.h"
#include "envoy/config/subscription.h"
#include "envoy/config/xds_resources_delegate.h"
#include "envoy/event/dispatcher.h"
#include "envoy/grpc/async_client.h"
#include "envoy/local_info/local_info.h"
#include "envoy/upstream/cluster_manager.h"

#include "source/extensions/config_subscription/grpc/grpc_subscription_impl.h"

namespace Envoy {
namespace Khulnasoft {

// Khulnasoft XDS API config source. Used for all Khulnasoft XDS.
extern envoy::config::core::v3::ConfigSource khulnasoft_xds_api_config;

std::unique_ptr<Config::GrpcSubscriptionImpl>
subscribe(const std::string& type_url, const LocalInfo::LocalInfo& local_info,
          Upstream::ClusterManager& cm, Event::Dispatcher& dispatcher,
          Random::RandomGenerator& random, Stats::Scope& scope,
          Config::SubscriptionCallbacks& callbacks,
          Config::OpaqueResourceDecoderSharedPtr resource_decoder,
          std::chrono::milliseconds init_fetch_timeout = std::chrono::milliseconds(0));

} // namespace Khulnasoft
} // namespace Envoy
