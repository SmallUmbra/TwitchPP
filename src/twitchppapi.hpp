#include "twitchpptypes.hpp"

namespace TwitchPP {
    const std::string TWITCH_API_BASE {"https://api.twitch.tv/helix/"};
    class TwitchAPI {
        protected:
            std::string m_app_access_token {""};
            std::string m_client_id {""};
            template<typename T>
            std::vector<T> string_to_vector_objects(std::string data_string) {
                std::pair<std::string, std::string> cycle {"", data_string};
                std::vector<T> elements {};
                do {
                    cycle = get_first_value(cycle.second);
                    if (cycle.first != "") {
                        elements.push_back(T(cycle.first));
                    }
                } while(cycle.second != "");
                return elements;
            }
            template<typename T>
            VectorResponse<T> process_response(Response<std::string>& response,
                                               const bool& is_single = false) {
                auto [data_string, leftovers] = get_first_value(response.data.substr(1, response.data.size() - 1));
                auto [cursor_string, _] = get_first_value(leftovers);
                std::string cursor = get_object_param("\"cursor\"", cursor_string);
                std::string message = !data_string.size() ? get_object_param("\"message\"", response.data) : "";
                if (is_single) {
                    return {{T(data_string)}, cursor, response.code, message, response.headers, leftovers};
                }
                std::vector<T> elements = this->string_to_vector_objects<T>(data_string);
                return {elements, cursor, response.code, message, response.headers, leftovers};
            }
        public:
            TwitchAPI(const std::string& app_access_token, const std::string& client_id);
            VectorResponse<TwitchGame> search_categories(std::string_view query,
                                                         std::optional<size_t> first = std::nullopt,
                                                         std::optional<std::string_view> starting_cursor = std::nullopt);
            VectorResponse<TwitchGame> get_games(std::string_view game_query,
                                                 const bool& search_by_name = false);
            VectorResponse<TwitchGame> get_games(const std::vector<std::string>& game_query,
                                                 const bool& search_by_name = false);
            VectorResponse<TwitchGame> get_top_games(std::optional<size_t> first = std::nullopt,
                                                     std::optional<std::string> starting_cursor = std::nullopt,
                                                     const bool& is_before = false);
            VectorResponse<TwitchUser> get_users(std::optional<std::string_view> user_query = std::nullopt,
                                                 std::optional<bool> search_by_login = false);
            VectorResponse<TwitchUser> get_users(const std::vector<std::string>& game_query,
                                                 const bool& search_by_login = false);
            VectorResponse<TwitchEmoteResponse> get_global_emotes();
            VectorResponse<TwitchChannelEmoteResponse> get_channel_emotes(std::string_view broadcaster_id);
            VectorResponse<TwitchBadgeSet> get_global_chat_badges();
            VectorResponse<TwitchBadgeSet> get_channel_chat_badges(std::string_view broadcaster_id);
            VectorResponse<TwitchChannelEmoteResponse> get_emote_sets(std::string_view emote_set_id);
            VectorResponse<TwitchChannelEmoteResponse> get_emote_sets(const std::vector<std::string>& emote_set_ids);
            VectorResponse<TwitchChannelInformation> get_channel_information(std::string_view broadcaster_id);
            VectorResponse<TwitchChatSettings> get_chat_settings(std::string_view broadcaster_id);
            VectorResponse<TwitchChannel> search_channels(std::string_view query,
                                                          std::optional<bool> live_only = std::nullopt,
                                                          std::optional<size_t> first = std::nullopt,
                                                          std::optional<std::string> starting_cursor = std::nullopt);
            VectorResponse<TwitchChannelTeam> get_channel_teams(std::string_view broadcaster_id);
            VectorResponse<TwitchStream> get_streams(std::optional<StreamsRequest> streams_request = std::nullopt);
            VectorResponse<TwitchUsersFollows> get_users_follows(std::string_view user_id,
                                                                 const bool& is_from = false,
                                                                 std::optional<size_t> first = std::nullopt,
                                                                 std::optional<std::string> after = std::nullopt);
            VectorResponse<TwitchClips> get_clips(ClipsRequest& clips_request);
            VectorResponse<TwitchVideos> get_videos(VideosRequest& videos_request);
            VectorResponse<TwitchChannelStreamSchedule> get_channel_stream_schedule(std::string_view broadcaster_id,
                                                                                    std::optional<OptionalScheduleRequest> options = std::nullopt);
            VectorResponse<TwitchTeam> get_teams(std::string_view team_query,
                                                 const bool& search_by_name = false);
            VectorResponse<TwitchTeam> get_teams(const std::vector<std::string>& team_query,
                                                 const bool& search_by_name = false);
            Response<std::string> get_channel_icalendar(std::string_view broadcaster_id);
            VectorResponse<TwitchCheermote> get_cheermotes(std::optional<std::string_view> broadcaster_id = std::nullopt);
            VectorResponse<TwitchBasicPlaylist> get_soundtrack_playlists();
            VectorResponse<TwitchPlaylist> get_soundtrack_playlist(std::string_view playlist_id);
            VectorResponse<TwitchCurrentTrack> get_soundtrack_current_track(std::string_view broadcaster_id);
            VectorResponse<TwitchStreamTag> get_all_stream_tags(const std::vector<std::string>& tag_ids = {},
                                                                std::optional<size_t> first = std::nullopt,
                                                                std::optional<std::string> after = std::nullopt);
            VectorResponse<TwitchStreamTag> get_stream_tags(std::string_view broadcaster_id);
    };
}