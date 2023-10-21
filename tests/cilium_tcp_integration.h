#pragma once

#include "test/integration/integration.h"

#include "tests/accesslog_server.h"

namespace Envoy {

class KhulnasoftTcpIntegrationTest : public BaseIntegrationTest,
                                 public testing::TestWithParam<Network::Address::IpVersion> {
public:
  KhulnasoftTcpIntegrationTest(const std::string& config);

  void createEnvoy() override;

  virtual std::string testPolicyFmt();

  void initialize() override;

  AccessLogServer accessLogServer_;
};

} // namespace Envoy
