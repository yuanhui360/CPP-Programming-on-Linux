#include <iostream>
#include <aws/core/Aws.h>
#include <aws/core/utils/Outcome.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/ListTablesRequest.h>
#include <aws/dynamodb/model/ListTablesResult.h>
#include <aws/core/client/ClientConfiguration.h>

void List_Table(int listLimit)
{
    Aws::Client::ClientConfiguration clientCFG;
    Aws::DynamoDB::DynamoDBClient dynamoClient(clientCFG);

    Aws::DynamoDB::Model::ListTablesRequest ltr;
    ltr.SetLimit(listLimit);
    do
    {
        const Aws::DynamoDB::Model::ListTablesOutcome& lto = dynamoClient.ListTables(ltr);
        if (!lto.IsSuccess())
        {
            std::cout << "Error: " << lto.GetError().GetMessage() << std::endl;
            exit(1);
        }

        for (const auto& s : lto.GetResult().GetTableNames())
            std::cout << s << std::endl;

        ltr.SetExclusiveStartTableName(lto.GetResult().GetLastEvaluatedTableName());

    } while (!ltr.GetExclusiveStartTableName().empty());
}

int main(int argc, char** argv)
{

    if (argc != 2)
    {
        std::cout << "Usage: list_table <limit> " << std::endl;
        return 1;
    }

    Aws::SDKOptions options;
    Aws::InitAPI(options);

    {
        List_Table(std::atoi(argv[1]));
    }

    Aws::ShutdownAPI(options);
    return 0;
}
