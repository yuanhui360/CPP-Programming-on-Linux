#include <iostream>
#include <string>

#include <aws/core/Aws.h>
#include <aws/core/utils/Outcome.h>
#include <aws/core/utils/logging/ConsoleLogSystem.h>
#include <aws/core/utils/logging/AWSLogging.h>

#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/AttributeDefinition.h>
#include <aws/dynamodb/model/UpdateItemRequest.h>
#include <aws/dynamodb/model/UpdateItemResult.h>
#include <aws/core/client/ClientConfiguration.h>

void Update_Item(const Aws::String table, const Aws::String id, const Aws::String setValue)
{
        Aws::Client::ClientConfiguration clientCFG;
        Aws::DynamoDB::DynamoDBClient dynamoClient(clientCFG);

        // Set up the request
        Aws::DynamoDB::Model::UpdateItemRequest req;
        req.SetTableName(table);
        Aws::DynamoDB::Model::AttributeValue hashKey;
        hashKey.SetS(id);
        req.AddKey("id", hashKey);

        Aws::String update_expression("SET #key1 = :Value1");
        req.SetUpdateExpression(update_expression);
        Aws::Vector<Aws::String> strVec = Aws::Utils::StringUtils::Split(setValue, '=');

        Aws::Map<Aws::String, Aws::String> mKey;
        mKey["#key1"] = strVec[0];
        req.SetExpressionAttributeNames(mKey);

        Aws::DynamoDB::Model::AttributeValue av;
        av.SetS(strVec[1]);
        Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> mValue;
        mValue[":Value1"] = av;
        req.SetExpressionAttributeValues(mValue);

        // Retrieve the item's fields and values
        const Aws::DynamoDB::Model::UpdateItemOutcome& result = dynamoClient.UpdateItem(req);
        if (result.IsSuccess())
        {
            // Reference the retrieved fields/values
            std::cout << "Update Success" << std::endl;
        }
        else
        {
            std::cout << "Failed to get item: " << result.GetError().GetMessage() << std::endl;;
        }
}

int main(int argc, char** argv)
{

    if (argc != 4)
    {
        std::cout << "Usage:  update_item <table> <id> <field=value>" << std::endl;
        return 1;
    }

    Aws::SDKOptions options;
    Aws::InitAPI(options);

    {
        const Aws::String table(argv[1]);
        const Aws::String id(argv[2]);
        const Aws::String sValue(argv[3]);
        Update_Item(table, id, sValue);
    }

    Aws::ShutdownAPI(options);
    return 0;
}
