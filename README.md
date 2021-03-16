# C++ Serverless Discord Bot
👋 Hi there! Welcome to a 💪 project of mine where I wanted to use my AWS and C++ knowledge to make a Discord bot that relies on [Slash Commands](https://discord.com/developers/docs/interactions/slash-commands) for input. Once things settle down a bit in this repo, I'll revisit the docs here to describe how to build and deploy this. For now, if you're interested or have questions, hop onto our [C++ Discord](https://discord.com/channels/216010419213434882/350062491088453634/350065188944478209) and ping `@kevinkjt2000#2000` in `#project-discussions`.

### Making a new bot on Discord's developer portal
The bot needs slash command permission, and the easiest way to generate a URL for it is from the "OAuth2 URL Generator" in the Discord Developer Portal when you have your app selected (You made a new app for testing this, right?). Navigate to the generated URL and add it to a testing server, and then follow along below.

### Preparing your environment
Install git, terraform, CMake, whatever build tool you plan to have CMake generate files for, and your favorite modern C++ compiler. (I only tested with make and gcc.)

Export these environment variables (personally, I use .envrc files that direnv automatically loads for me):
```
export BOT_TOKEN="<get your own>"
export CLIENT_ID="<this is a number>"
export GUILD_ID="<are you a discord dev or not?>"
export PUBLIC_KEY="<public ed25519 key for your app>"
export TF_VAR_public_key="${PUBLIC_KEY}"
```

### How to build/deploy
Just once, you'll need to clone repos and setup my hunter fork (because it has the aws C++ lambda runtime packaged):
```
git clone https://github.com/kevinkjt2000/serverless-discord.git
git clone https://github.com/kevinkjt2000/hunter.git
cd hunter
git checkout pr.aws_lambda_cpp
cd ../serverless-discord
```

Generate build files:
```
cmake -H. -Bbuild -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug -DHUNTER_ROOT=$(pwd)/../hunter
terraform init
```

Building, testing, and deploying:
```
(cd build && make && ./tests && (yes | make aws-lambda-package-serverless-discord)) && terraform apply -auto-approve
```

After the app is deployed at least once, you can add the URL of the API Gateway stage to the settings on the Discord Developer Portal (specifically the "Interactions Endpoint URL"). *fingers crossed* that all went well.
