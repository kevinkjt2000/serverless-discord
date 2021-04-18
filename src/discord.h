#pragma once
#include <string>
#include <variant>
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

    enum class ApplicationCommandOptionType {
        SubCommand = 1,
        SubCommandGroup = 2,
        String = 3,
        Integer = 4,
        Boolean = 5,
        User = 6,
        Channel = 7,
        Role = 8,
    };

    using Snowflake = std::string;

    struct GuildMemberIncludingPermissions {};
    struct DiscordChannel {};
    struct DiscordRole {};
    struct DiscordUser {};

    struct ApplicationCommandInteractionDataResolved {
        std::optional<std::string> users;
        std::optional<std::string> members;
        std::optional<std::string> roles;
        std::optional<std::string> channels;
    };

    struct ApplicationCommandInteractionDataOption {
        std::string name;
        ApplicationCommandOptionType type;

        //TODO find out if this is optional
        std::variant<std::string, int, bool, DiscordUser, DiscordChannel, DiscordRole> value; 
        std::vector<ApplicationCommandInteractionDataOption> options;
    };

    struct ApplicationCommandInteractionData {
        Snowflake id;
        std::string name;
        std::optional<ApplicationCommandInteractionDataResolved> resolved;
        std::vector<ApplicationCommandInteractionDataOption> options;
    };

    struct Interaction {
        Snowflake id;
        Snowflake application_id;
        std::optional<Snowflake> guild_id;
        std::optional<Snowflake> channel_id;
        std::string token;
        const int version = 1;

        struct Ping {
            InteractionType type = InteractionType::Ping;
        };
        struct ApplicationCommand {
            InteractionType type = InteractionType::ApplicationCommand;
            ApplicationCommandInteractionData data;
        };
        std::variant<Ping, ApplicationCommand> m_interaction;
        
        struct FromGuild {
            GuildMemberIncludingPermissions member;
        };
        struct FromDM {
            DiscordUser user;
        };
        std::variant<FromGuild, FromDM> m_source;
    };
    void to_json(json& j, const Interaction& interaction);

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
