#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <aws/core/Aws.h>
#include <aws/core/utils/Outcome.h>
#include <aws/core/utils/logging/ConsoleLogSystem.h>
#include <aws/core/utils/logging/AWSLogging.h>
#include <aws/core/utils/json/JsonSerializer.h>

#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/AttributeDefinition.h>
#include <aws/dynamodb/model/PutItemRequest.h>
#include <aws/dynamodb/model/PutItemResult.h>
#include <aws/core/client/ClientConfiguration.h>

/*
 * JSON file example
 *
 * {
 *     "id"         : "1233",
 *     "first_name" : "John",
 *     "last_name"  : "Pasquale",
 *     "gender"     : "Male",
 *     "level"      : "Beginner"
 * }
 *
 * std::shared_ptr<Aws::IOStream> ssJson = Aws::MakeShared<Aws::StringStream>("");
 */

Aws::StringStream loadFile(std::string& mFileName)
{
    Aws::StringStream ss;
    std::string line;
    std::ifstream myFile(mFileName);
    if (myFile.is_open()) {
        while ( getline(myFile, line)) {
            ss << line;
        }
    }
    else {
        std::cout << "Fialed of open file : " << mFileName << std::endl;
        exit(2);
    }
    myFile.close();
    return ss;
}

int loadJsonFile(const Aws::String& table, std::string fileName)
{
    Aws::StringStream ssJson = loadFile(fileName);
    Aws::Utils::Json::JsonValue mJson(ssJson);
    if ( mJson.View().IsNull() ) {
        return 1;
    }
    //  Aws::Map<Aws::String, JsonView>
    const auto& jObjs = mJson.View().AsObject().GetAllObjects();

    Aws::Client::ClientConfiguration clientCFG;
    Aws::DynamoDB::DynamoDBClient dynamoClient(clientCFG);

    // Set up the request
    Aws::DynamoDB::Model::PutItemRequest req;
    req.SetTableName(table);
    Aws::DynamoDB::Model::AttributeValue av;

    for (const auto& mObj : jObjs) {

        Aws::String sKey = mObj.first.c_str();
        Aws::String sVal = mObj.second.AsString().c_str();

        std::cout << sKey << " : " << sVal << std::endl;
        // Aws::DynamoDB::Model::AttributeValue av;
        av.SetS(sVal);
        req.AddItem(sKey, av);
    }

    // Put Item Request into table
    const Aws::DynamoDB::Model::PutItemOutcome& result = dynamoClient.PutItem(req);
    if (result.IsSuccess()) {
        std::cout << "Load JSON file successfully!" << std::endl;
    }
    else {
        std::cout << result.GetError().GetMessage() << std::endl;
        return 1;
    }
    return 0;
}

int main(int argc, char** argv)
{

    int rc = 0;
    if (argc != 3)
    {
        std::cout << "Usage: load_json_file <table> <json_file_name> " << std::endl;
        return 1;
    }

    Aws::SDKOptions options;
    Aws::InitAPI(options);

    {
        const Aws::String table(argv[1]);
        const std::string fileName(argv[2]);
        rc = loadJsonFile(table, fileName);
    }

    Aws::ShutdownAPI(options);
    return rc;
}
          
