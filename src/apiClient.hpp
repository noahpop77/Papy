#pragma once

#include <string>

// project dependencies
#include "dependencies/json.hpp"

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "dependencies/httplib.h"

class apiClient {
public:
    explicit apiClient(const std::string& serverAddress);

    void setEndpoint(const std::string& endpoint);
    void setParameter(const std::string& parameter);
    void setPayload(const nlohmann::json& payload);

    std::string sendGETRequest(const std::string& bearer, const std::string& authorization) const;
    std::string sendPOSTRequest(const std::string& bearer, const std::string& authorization) const;

private:
    std::unique_ptr<httplib::Client> client;
    std::unique_ptr<httplib::SSLClient> sslClient;

    std::string endpoint;
    std::string parameter;
    nlohmann::json payload;

    static std::string errorToString(httplib::Error err);
};
