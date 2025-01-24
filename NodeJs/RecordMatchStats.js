import { CognitoIdentityProviderClient, AdminGetUserCommand } from "@aws-sdk/client-cognito-identity-provider"; // ES Modules import
import { DynamoDBClient, GetItemCommand, PutItemCommand } from "@aws-sdk/client-dynamodb"; // ES Modules import
import { marshall, unmarshall } from "@aws-sdk/util-dynamodb";

export const handler = async (event) => {

  const MyCognitoIdentityProviderClient = new CognitoIdentityProviderClient({ region: process.env.REGION });

  const getUserInput = {
    UserPoolId: process.env.USER_POOL_ID,
    Username: event.userName
  };
  const getUserCommand = new AdminGetUserCommand(getUserInput);

  try {
    const getUserResponse = await MyCognitoIdentityProviderClient.send(getUserCommand);

    const userAttributes = getUserResponse.UserAttributes;
    const sub = userAttributes.find(attribute => attribute.Name === 'sub').Value;
    const email = userAttributes.find(attribute => attribute.Name === 'email').Value;

    const MyDynamoDBClient = new DynamoDBClient({ region: process.env.REGION });
    const getItemInput = {
      TableName: process.env.TABLE_NAME,
      Key: marshall({ "databaseid": sub })
    };

    const getItemCommand = new GetItemCommand(getItemInput);

    let statsFromDB = await MyDynamoDBClient.send(getItemCommand);
    statsFromDB = statsFromDB.Item ? unmarshall(statsFromDB.Item) : null;

    // {
    //   "userName": "test001",
    //   "matchStats": {
    //     "hits": 1,
    //     "misses": 12
    //   }
    // }

    const matchStats = event.matchStats;
    for (const key in matchStats) {
      const value = matchStats[key];
      if (statsFromDB && statsFromDB[key]) {
        statsFromDB[key] += value;
      }
      else {
        statsFromDB[key] = value;
      }
    }

    const putItemInput = {
      TableName: process.env.TABLE_NAME,
      Item: marshall({...statsFromDB})
    };

    const putItemCommand = new PutItemCommand(putItemInput);
    const putItemResponse = await MyDynamoDBClient.send(putItemCommand);
    
    return {
      statusCode: 200,
      body: `Updated match stats for ${event.userName}`
    }
    
  }
  catch (error) {
    return error;
  }

};
