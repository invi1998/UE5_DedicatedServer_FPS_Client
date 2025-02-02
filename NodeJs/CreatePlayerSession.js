import { GameLiftClient, CreatePlayerSessionCommand } from "@aws-sdk/client-gamelift"; // ES Modules import


export const handler = async (event) => {
  
	const OwnerGameliftClient = new GameLiftClient({ region: process.env.REGION });
	
	try {

		const createPlayerSessionInput = {
			GameSessionId: event.gameSessionId,
			PlayerId: event.playerId,
			// Location: "custom-home-desktop"	// ECS 需要移除该配置
		};

		const createPlayerSessionCommand = new CreatePlayerSessionCommand(createPlayerSessionInput);
		const createPlayerSessionResponse = await OwnerGameliftClient.send(createPlayerSessionCommand);
		return createPlayerSessionResponse.PlayerSession;
	}
	catch (error) {
		return error;
	}

};
