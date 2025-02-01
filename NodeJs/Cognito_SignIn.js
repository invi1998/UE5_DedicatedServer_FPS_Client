import { CognitoIdentityProviderClient, InitiateAuthCommand, GetUserCommand } from "@aws-sdk/client-cognito-identity-provider"; // ES Modules import


export const handler = async (event) => {
  
	const cognitoClient = new CognitoIdentityProviderClient({ region: process.env.REGION });

	const {username, password, refreshToken } = event;

	if (refreshToken) {
		const refreshTokenInput = {
			ClientId: process.env.CLIENT_ID,
			AuthFlow: 'REFRESH_TOKEN_AUTH',
			AuthParameters: {
				REFRESH_TOKEN: refreshToken
			}
		};
		const refreshTokenCommand = new InitiateAuthCommand(refreshTokenInput);
		try {
			const refreshTokenResponse = await cognitoClient.send(refreshTokenCommand);
			return refreshTokenResponse;
		} catch (error) {
			return error;
		}
	} else {
		const SignInInput = {
			AuthFlow: "USER_PASSWORD_AUTH",
			ClientId: process.env.CLIENT_ID,
			AuthParameters: {
				USERNAME: username,
				PASSWORD: password,
			},
		};
	
		try {
			const SignInCommand = new InitiateAuthCommand(SignInInput);
			const SignInResponse = await cognitoClient.send(SignInCommand);

			// 获取登录角色的信息
			const getUserInput = {
				AccessToken: SignInResponse.AuthenticationResult.AccessToken,
			};
			const getUserCommand = new GetUserCommand(getUserInput);
			const getUserResponse = await cognitoClient.send(getUserCommand);

			// 获取Email
			const email = getUserResponse.UserAttributes.find((attr) => attr.Name === "email").Value;

			return {
				...SignInResponse,
				User: {
					Username: username,
					Email: email,
				},
			};
		} catch (error) {
			return error;
		}
	
	}
};
