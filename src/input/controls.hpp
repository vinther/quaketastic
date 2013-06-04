#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include <cstdint>

#include "../game/game_state.hpp"
#include "../scene/camera.hpp"

namespace std
{
    template <>
    struct hash<std::tuple<int32_t, int8_t, int8_t>>
    {
        inline size_t operator()(const std::tuple<int32_t, int8_t, int8_t>& v) const
        {
            return hash<int64_t>()(uint64_t(std::get<0>(v)) << 32 | std::get<1>(v) << 8 | std::get<2>(v) );
        }
    };
}

namespace qts {
    typedef std::unordered_map<std::tuple<int32_t, int8_t, int8_t>, std::function<void(game_state&)>> keybinding_map;
    //typedef std::unordered_map<std::tuple<int32_t, int32_t>, std::function<void(game_state&)>> mouse_button_binding_map;
    typedef std::unordered_set<int32_t> key_set;
    typedef std::unordered_set<int32_t> mouse_button_set;

    namespace controls {
        const static int8_t KEYBOARD = 0;
        const static int8_t MOUSE_BUTTON = 1;

        struct fixed_movement_effect {
            inline void operator()(game_state& g) const {
                g.active_camera->eye += displacement * g.active_camera->rotation * 4.5f;
            }

            fixed_movement_effect(const glm::vec3& displacement)
                : displacement(displacement)
            {}

            const glm::vec3 displacement;
        };

        inline std::function<void(game_state&)> fixed_movement(const std::string& s, const glm::vec3& v)
        {
            return [=](game_state& g){ g.active_effects[s] = fixed_movement_effect(v); };
        }

        inline std::function<void(game_state&)> cancel_effect(const std::string& s)
        {
            return [=](game_state& g){ g.active_effects.erase(s); };
        }
    }
}

#endif // CONTROLS_HPP



