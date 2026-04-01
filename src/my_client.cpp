#include <iostream>
#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>
#include <open62541/client_subscriptions.h>
#include <open62541/client.h>
#include <open62541/types.h>

int main(int argc, char *argv[]) {
    const char *endpoint = "opc.tcp://localhost:4840";
    if (argc > 1) {
        endpoint = argv[1];
    }

    std::cout << "[my_client] endpoint=" << endpoint << std::endl;

    UA_Client *client = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(client));

    UA_StatusCode status = UA_Client_connect(client, endpoint);
    if (status != UA_STATUSCODE_GOOD) {
        std::cerr << "连接失败: " << UA_StatusCode_name(status) << " (" << status << ")" << std::endl;
        UA_Client_delete(client);
        return static_cast<int>(status);
    }

    // 读取服务器当前时间
    UA_Variant value;
    UA_Variant_init(&value);
    UA_NodeId nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME);
    status = UA_Client_readValueAttribute(client, nodeId, &value);

    if (status == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_DATETIME])) {
        UA_DateTime serverTime = *(UA_DateTime *)value.data;
        UA_DateTimeStruct ts = UA_DateTime_toStruct(serverTime);
        std::cout << "Server Current Time: "
                  << ts.year << "-" << ts.month << "-" << ts.day << " "
                  << ts.hour << ":" << ts.min << ":" << ts.sec << "." << ts.milliSec
                  << std::endl;
    } else {
        std::cerr << "读取时间失败: " << UA_StatusCode_name(status) << " (" << status << ")" << std::endl;
    }

    UA_Variant_clear(&value);

    // 读取一个节点值示例（可替换为你的 NodeId）
    UA_NodeId readNode = UA_NODEID_STRING(1, const_cast<char*>("Demo.Static.Scalar.String"));
    status = UA_Client_readValueAttribute(client, readNode, &value);
    if (status == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_STRING])) {
        UA_String val = *(UA_String *)value.data;
        std::cout << "Value(" << "ns=1;s=Demo.Static.Scalar.String" << ") = "
                  << std::string(reinterpret_cast<char*>(val.data), val.length) << std::endl;
    } else {
        std::cerr << "读取节点值失败: " << UA_StatusCode_name(status) << " (" << status << ")" << std::endl;
    }

    UA_Variant_clear(&value);

    UA_Client_disconnect(client);
    UA_Client_delete(client);

    std::cout << "[my_client] 退出" << std::endl;
    return 0;
}
