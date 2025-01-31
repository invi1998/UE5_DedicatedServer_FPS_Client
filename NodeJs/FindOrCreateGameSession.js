import { GameLiftClient, ListFleetsCommand, DescribeFleetAttributesCommand, DescribeGameSessionsCommand, CreateGameSessionCommand   } from "@aws-sdk/client-gamelift"; // ES Modules import

export const handler = async (event) => {
  // TODO implement
  const gameliftClient = new GameLiftClient({ region: process.env.REGION });

  try {
    // 1：获取舰队列表
    const listFleetsInput = {
      Limit: Number(process.env.FLEETS_NUM_LIMIT)
    };
  
    const listFleetsCommand = new ListFleetsCommand(listFleetsInput);
    const listFleetsResponse = await gameliftClient.send(listFleetsCommand);
  
    const DescribeFleetAttributesInput = {
      FleetIds: listFleetsResponse.FleetIds,
      Limit: Number(process.env.FLEETS_NUM_LIMIT)
    };
    
    // 2：获取舰队列表信息，然后遍历查找一个活跃可用的服务器
    const describeFleetAttributesCommand = new DescribeFleetAttributesCommand(DescribeFleetAttributesInput);
    const describeFleetAttributesResponse = await gameliftClient.send(describeFleetAttributesCommand);

    const fleetAttributes = describeFleetAttributesResponse.FleetAttributes;

    let fleetId;
    for (const fleetAttribute of fleetAttributes) {
      if (fleetAttribute.Status === "ACTIVE") {
        fleetId = fleetAttribute.FleetId;
        break;
      }
    }

    // 3： 查找服务器
    const describeGameSessionsInput = {
      FleetId: fleetId,
      Limit: Number(process.env.GAME_SESSIONS_NUM_LIMIT),
      StatusFilter: "ACTIVE"           // 过滤仅活跃的会话
      // Location: "custom-home-desktop"   // 如果是EC2服务器，需要将此配置项移除
    };
    const describeGameSessionsCommand = new DescribeGameSessionsCommand(describeGameSessionsInput);
    const describeGameSessionsResponse = await gameliftClient.send(describeGameSessionsCommand);
    
    let gameSession;
    for (const session of describeGameSessionsResponse.GameSessions) {
      if (session.CurrentPlayerSessionCount < session.MaximumPlayerSessionCount && session.PlayerSessionCreationPolicy === "ACCEPT_ALL") {
        gameSession = session;
        break;
      }
    }

    if (gameSession) {
      // 找到一个可用的游戏会话
    }
    else {
      // 如果没有可用的会话，创建一个新的
      const createGameSessionInput = {
        FleetId: fleetId,
        MaximumPlayerSessionCount: Number(process.env.GAME_SESSIONS_NUM_LIMIT),
        PlayerSessionCreationPolicy: "ACCEPT_ALL",
        // Location: "custom-home-desktop",   // 如果是EC2服务器，需要将此配置项移除
        GameProperties: [
          {
            Key: "difficulty",
            Value: "novice"
          }
        ]
      };

      const createGameSessionCommand = new CreateGameSessionCommand(createGameSessionInput);
      const createGameSessionResponse = await gameliftClient.send(createGameSessionCommand);

      if (createGameSessionResponse.GameSession) {
        gameSession = createGameSessionResponse.GameSession;
      }

    }

    return gameSession;

  }
  catch (error) {
    return error;
  }
  


};
