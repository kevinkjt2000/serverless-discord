#pragma once
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Discord {
    enum class InteractionType {
        Ping = 1,
        ApplicationCommand = 2,
    };

    enum class InteractionResponseType {
        Pong = 1,
        // 2 and 3 are deprecated
        ChannelMessageWithSource = 4,
        DeferredChannelMessageWithSource = 5,
    };

    using Snowflake = std::string;

    struct AllowedMentions {
        std::vector<std::string> parse;
        std::vector<Snowflake> roles;
        std::vector<Snowflake> users;
        bool replied_user = false;
    };

    struct InteractionApplicationCommandCallbackData {
        bool tts;
        std::string content;
        //TODO: std::vector<Embed> embeds;
        AllowedMentions allowed_mentions;
        int flags;
    };
    void to_json(json& j, const InteractionApplicationCommandCallbackData& data);

    struct InteractionResponse {
        InteractionResponseType type;
        InteractionApplicationCommandCallbackData data;
    };
    void to_json(json& j, const InteractionResponse& resp);
}
