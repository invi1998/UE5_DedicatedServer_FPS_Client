import { CognitoIdentityProviderClient, ConfirmSignUpCommand } from "@aws-sdk/client-cognito-identity-provider"; // ES Modules import


export const handler = async (event) => {

    const CognitoClient = new CognitoIdentityProviderClient({region: process.env.REGION});

    const cognitoConfirmSignUpInput = {
        ClientId: process.env.CLIENT_ID,
        Username: event.username,
        ConfirmationCode: event.code
    };
    
    try {
        const cognitoConfirmSignUpCommand = new ConfirmSignUpCommand(cognitoConfirmSignUpInput);
        const cognitoConfirmSignUpResponse = await CognitoClient.send(cognitoConfirmSignUpCommand);
        return cognitoConfirmSignUpResponse;
    } catch (error) {
        return error;
    }


};
