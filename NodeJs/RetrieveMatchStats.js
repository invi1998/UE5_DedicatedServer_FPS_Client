import { CognitoIdentityProviderClient, GetUserCommand } from "@aws-sdk/client-cognito-identity-provider"; // ES Modules import
import { DynamoDBClient, GetItemCommand } from "@aws-sdk/client-dynamodb"; // ES Modules import
import { marshall, unmarshall } from "@aws-sdk/util-dynamodb";

export const handler = async (event) => {

    const MyCognitoIdentityProviderClient = new CognitoIdentityProviderClient({region: process.env.REGION});
    const MyDynamoDBClient = new DynamoDBClient({region: process.env.REGION});

    try {
        console.log(event);

        const getUserInput = {
            AccessToken: event.accessToken
        };
        const user = await MyCognitoIdentityProviderClient.send(new GetUserCommand(getUserInput));
        const sub = user.UserAttributes.find(o => o.Name === 'sub').Value;

        const getItemInput = {
            TableName: process.env.TABLE_NAME,
            Key: marshall({ databaseid: sub })
        };
        const getItemOutput = await MyDynamoDBClient.send(new GetItemCommand(getItemInput));
        const playerStats = getItemOutput.Item ? unmarshall(getItemOutput.Item) : {};
        console.log(playerStats);
        return playerStats;

    } catch (error) {
        console.log(error);
        return error;
    }

};
