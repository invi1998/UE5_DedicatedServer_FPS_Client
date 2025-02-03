import { CognitoIdentityProviderClient, ListUsersCommand } from "@aws-sdk/client-cognito-identity-provider"; // ES Modules import


export const handler = async (event) => {
  
	const CognitoClient = new CognitoIdentityProviderClient({ region: process.env.REGION });

	const email = event.request.userAttributes.email;

	const ListUsersInput = {
		UserPoolId: event.userPoolId,
		AttributesToGet: ["email"],
		Filter: `email = \"${email}\"`
	};

	const signUpListUsersCommand = new ListUsersCommand(ListUsersInput);

	try {

		const response = await CognitoClient.send(signUpListUsersCommand);

		if (response.Users.length > 0) {

			throw new Error("EmailExistsException");

		}

		return event;

	} catch (error) {

		throw new Error(error.message);

	}

};
