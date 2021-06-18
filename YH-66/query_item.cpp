#include <iostream>
#include <string>

#include <aws/core/Aws.h>
#include <aws/core/utils/Outcome.h>
#include <aws/core/utils/logging/ConsoleLogSystem.h>
#include <aws/core/utils/logging/AWSLogging.h>

#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/AttributeDefinition.h>
#include <aws/dynamodb/model/QueryRequest.h>
#include <aws/dynamodb/model/QueryResult.h>
#include <aws/core/client/ClientConfiguration.h>

typedef Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> myMapType;

void query_item(Aws::String table, Aws::String id)
{
    Aws::Client::ClientConfiguration clientCFG;
    Aws::DynamoDB::DynamoDBClient dynamoClient(clientCFG);

    // Set up the request
    Aws::DynamoDB::Model::QueryRequest    req;
    req.SetTableName(table);
    Aws::String querykeyCondition("#key1 = :v1");
    req.SetKeyConditionExpression(querykeyCondition);

    Aws::Map<Aws::String, Aws::String> mCdtKey;
    mCdtKey["#key1"] = "id";
    req.SetExpressionAttributeNames(mCdtKey);

    // Aws::Vector<Aws::String> strVec = Aws::Utils::StringUtils::Split(id.c_str(), '=');
    Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> mCdtVal;
    Aws::DynamoDB::Model::AttributeValue av;
    av.SetS(id);
    mCdtVal[":v1"] = av;
    req.SetExpressionAttributeValues(mCdtVal);

    // Retrieve the item's fields and values
    const Aws::DynamoDB::Model::QueryOutcome& result = dynamoClient.Query(req);
    if (result.IsSuccess())
    {
        // Reference the retrieved fields/values
        const Aws::Vector<myMapType>& resVec = result.GetResult().GetItems();
        if (resVec.size() > 0)
        {
            // Output each retrieved field and its value
            for (auto item : resVec) {
                for ( const auto& i : item )
                    std::cout << i.first << ": " << i.second.GetS() << std::endl;
                std::cout << "----------" << std::endl;
            }
        }
        else
        {
            std::cout << "No item found in table : " << table << std::endl;
        }
    }
    else
    {
        std::cout << "Failed to get item: " << result.GetError().GetMessage() << std::endl;;
    }
}

int main(int argc, char** argv)
{

    if (argc != 3)
    {
        std::cout << "Usage:  query_item <table> <id>" << std::endl;
        return 1;
    }

    Aws::SDKOptions options;
    Aws::InitAPI(options);

    {
        const Aws::String table(argv[1]);
        const Aws::String id(argv[2]);
        query_item(table, id);
    }

    Aws::ShutdownAPI(options);
    return 0;
}

