import { DynamoDBClient, PutItemCommand } from "@aws-sdk/client-dynamodb"; // ES Modules import

export const handler = async (event) => {

  if (event.triggerSource == "PostConfirmation_ConfirmSignUp") {
    const userName = event.userName;
    const cognitosub = event.request.userAttributes.sub;
    const email = event.request.userAttributes.email;

    const MyDynamoDBClient = new DynamoDBClient({ region: process.env.REGION });

    const inputItem = {
      TableName: process.env.TABLE_NAME,    // Players
      Item: {
        "databaseid": { "S": cognitosub },  // PK
        // "SK": { "S": "USER" },              // SK 排序键（如果有的话）
        "userName": { "S": userName },
        "email": { "S": email },
      }
    };
    const putItemCommand = new PutItemCommand(inputItem);

    try {
      const putItemResponse = await MyDynamoDBClient.send(putItemCommand);
    }
    catch (error) {
      return error;
    }

  }

  return event;

};

