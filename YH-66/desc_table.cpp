#include <iostream>
#include <aws/core/Aws.h>
#include <aws/core/utils/Outcome.h>
#include <aws/core/utils/logging/ConsoleLogSystem.h>
#include <aws/core/utils/logging/AWSLogging.h>

#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/AttributeDefinition.h>
#include <aws/dynamodb/model/DescribeTableRequest.h>
#include <aws/dynamodb/model/DescribeTableResult.h>
#include <aws/core/client/ClientConfiguration.h>

void desc_table(const Aws::String table)
{
    Aws::Client::ClientConfiguration clientCFG;
    Aws::DynamoDB::DynamoDBClient dynamoClient(clientCFG);

    Aws::DynamoDB::Model::DescribeTableRequest dtr;
    dtr.SetTableName(table);
    const Aws::DynamoDB::Model::DescribeTableOutcome& result = dynamoClient.DescribeTable(dtr);
    if (result.IsSuccess())
    {
        const Aws::DynamoDB::Model::TableDescription& td = result.GetResult().GetTable();
        std::cout << "Table name : " << td.GetTableName() << std::endl;
        std::cout << "Table ARN : " << td.GetTableArn() << std::endl;
        std::cout << "Status : " <<
        Aws::DynamoDB::Model::TableStatusMapper::GetNameForTableStatus(td.GetTableStatus()) << std::endl;
        std::cout << "Item count : " << td.GetItemCount() << std::endl;
        std::cout << "Size (bytes): " << td.GetTableSizeBytes() << std::endl;
        const Aws::DynamoDB::Model::ProvisionedThroughputDescription& ptd =
        td.GetProvisionedThroughput();
        std::cout << "Throughput" << std::endl;
        std::cout << " Read Capacity : " << ptd.GetReadCapacityUnits() << std::endl;
        std::cout << " Write Capacity: " << ptd.GetWriteCapacityUnits() << std::endl;
        const Aws::Vector<Aws::DynamoDB::Model::AttributeDefinition>& ad =
        td.GetAttributeDefinitions();
        std::cout << "Attributes" << std::endl;
        for (const auto& a : ad)
            std::cout << " " << a.GetAttributeName() << " (" <<
            Aws::DynamoDB::Model::ScalarAttributeTypeMapper::GetNameForScalarAttributeType(a.GetAttributeType())
            << ")" << std::endl;
    }
    else
    {
        std::cout << "Failed to describe table: " << result.GetError().GetMessage();
    }
}

int main(int argc, char** argv)
{

    if (argc != 2)
    {
        std::cout << "Usage: desc_table <table> " << std::endl;
        return 1;
    }

    Aws::SDKOptions options;
    Aws::InitAPI(options);

    {
        const Aws::String table(argv[1]);
        desc_table(table);
    }

    Aws::ShutdownAPI(options);
    return 0;
}
