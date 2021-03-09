from os import environ
import requests


# https://discord.com/api/oauth2/authorize?client_id=818213948481994804&permissions=2147485696&scope=applications.commands%20bot
app_id = environ["CLIENT_ID"]
guild_id = environ["GUILD_ID"]
bot_token = environ["BOT_TOKEN"]
url = f"https://discord.com/api/v8/applications/{app_id}/guilds/{guild_id}/commands"

json = {
    "name": "blep",
    "description": "Send a random adorable animal photo",
    "options": [
        {
            "name": "animal",
            "description": "The type of animal",
            "type": 3,
            "required": True,
            "choices": [
                {
                    "name": "Dog",
                    "value": "animal_dog"
                },
                {
                    "name": "Cat",
                    "value": "animal_cat"
                },
                {
                    "name": "Penguin",
                    "value": "animal_penguin"
                }
            ]
        },
        {
            "name": "only_smol",
            "description": "Whether to show only baby animals",
            "type": 5,
            "required": False
        }
    ]
}

headers = {
    "Authorization": f"Bot {bot_token}"
}

r = requests.post(url, headers=headers, json=json)
print(r.text)
print(r.status_code)
