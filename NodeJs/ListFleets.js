import { GameLiftClient, ListFleetsCommand } from "@aws-sdk/client-gamelift"; // ES Modules import

export const handler = async (event) => {
  
  const client = new GameLiftClient({ region: "ap-southeast-2" });

  try{

    const input = {};
    const command = new ListFleetsCommand(input);
    const response = await client.send(command);
  
    return response;

  }catch(e){
    return e;
  }
};
