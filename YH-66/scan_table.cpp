#include <iostream>
#include <aws/core/Aws.h>
#include <aws/core/utils/Outcome.h>
#include <aws/core/utils/logging/ConsoleLogSystem.h>
#include <aws/core/utils/logging/AWSLogging.h>

#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/AttributeDefinition.h>
#include <aws/dynamodb/model/ScanRequest.h>
#include <aws/dynamodb/model/ScanResult.h>
#include <aws/core/client/ClientConfiguration.h>

/*
 *  typedef Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> myMapType;
 *  const Aws::Vector<Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>>& itemVec =
 *      result.GetResult().GetItems();
 */

typedef Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> myMapType;

void ScanTable(const Aws::String table, const Aws::String filter)
{
    Aws::Client::ClientConfiguration clientCFG;
    Aws::DynamoDB::DynamoDBClient dynamoClient(clientCFG);

    // Set up the request
    Aws::DynamoDB::Model::ScanRequest req;
    req.SetTableName(table);
    req.SetLimit(50);

    if ( filter.length() > 0 ) {
        Aws::String filterCondition("#key1 = :v1");
        req.SetFilterExpression(filterCondition);
        Aws::Vector<Aws::String> strVec = Aws::Utils::StringUtils::Split(filter, '=');

        Aws::Map<Aws::String, Aws::String> mCdtKey;
        mCdtKey["#key1"] = strVec[0];
        req.SetExpressionAttributeNames(mCdtKey);

        Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> mCdtVal;
        Aws::DynamoDB::Model::AttributeValue av;
        av.SetS(strVec[1]);
        mCdtVal[":v1"] = av;
        req.SetExpressionAttributeValues(mCdtVal);
    }

    // Retrieve the table result
    const Aws::DynamoDB::Model::ScanOutcome& result =  dynamoClient.Scan(req);
    if (result.IsSuccess())
    {
        // Reference the retrieved items lists into vector
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
        std::cout << "Failed to get item: " << result.GetError().GetMessage();
    }
}

int main(int argc, char** argv)
{

    if (argc < 2 || argc > 3)
    {
        std::cout << "Usage:  scan_table <table> <field=valer>" << std::endl;
        return 1;
    }

    Aws::SDKOptions options;
    Aws::InitAPI(options);

    {
        Aws::String table(argv[1]);
        Aws::String filter;
        if (argc == 3)
            filter = Aws::String(argv[2]);
        ScanTable(table, filter);
    }

    Aws::ShutdownAPI(options);
    return 0;
}
