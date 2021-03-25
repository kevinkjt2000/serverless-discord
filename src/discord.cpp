#include <nlohmann/json.hpp>

#include "discord.h"

using json = nlohmann::json;

namespace Discord {
    void to_json(json& j, const InteractionApplicationCommandCallbackData& data) {
        j = json{{"tts", data.tts}, {"content", data.content}, {"flags", data.flags}}; //TODO: the rest of fields
    }

    void to_json(json& j, const InteractionResponse& resp) {
        j = json{{"type", resp.type}, {"data", resp.data}};
    }
}
