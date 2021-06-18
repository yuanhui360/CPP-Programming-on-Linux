#include <iostream>
#include <aws/core/Aws.h>
#include <aws/core/utils/Outcome.h>
#include <aws/core/utils/logging/ConsoleLogSystem.h>
#include <aws/core/utils/logging/AWSLogging.h>

#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/AttributeDefinition.h>
#include <aws/dynamodb/model/GetItemRequest.h>
#include <aws/core/client/ClientConfiguration.h>

void GetItem(const Aws::String table, const Aws::String id)
{
    Aws::Client::ClientConfiguration clientCFG;
    Aws::DynamoDB::DynamoDBClient dynamoClient(clientCFG);

    // Set up the request
    Aws::DynamoDB::Model::GetItemRequest req;
    req.SetTableName(table);
    Aws::DynamoDB::Model::AttributeValue hashKey;
    hashKey.SetS(id);
    req.AddKey("id", hashKey);

    // Retrieve the item's fields and values
    const Aws::DynamoDB::Model::GetItemOutcome& result = dynamoClient.GetItem(req);
    if (result.IsSuccess())
    {
        // Reference the retrieved fields/values
        const Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>& item = result.GetResult().GetItem();
        if (item.size() > 0)
        {
            // Output each retrieved field and its value
            for (const auto& i : item)
                std::cout << i.first << ": " << i.second.GetS() << std::endl;
        }
        else
        {
            std::cout << "No item found with the key " << id << std::endl;
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
        std::cout << "Usage:  get_item <table> <id> " << std::endl;
        return 1;
    }

    Aws::SDKOptions options;
    Aws::InitAPI(options);

    {
        const Aws::String table(argv[1]);
        const Aws::String id(argv[2]);
        GetItem(table, id);
    }

    Aws::ShutdownAPI(options);
    return 0;
}
