#include "game.h"

Game::Game(Queue<std::shared_ptr<GenericMsg>>& recv, const std::vector<std::string>& player_names,
           bool is_testing, bool is_cheating,
           SendQueuesMonitor<std::shared_ptr<GenericMsg>>& senders,
           std::shared_ptr<std::set<uint>> ids):
        senders(senders),
        recv(recv),
        player_names(player_names),
        ids(ids),
        game_over(false),
        is_testing(is_testing),
        players_created(false),
        map_manager(),
        is_cheating(is_cheating) {}

std::map<std::string, Player*> Game::generate_players(const std::vector<std::string>& names,
                                                      const PlayerObserver& obs, Map& map) {
    std::vector<Coordinate> coordinates = map.get_players_spawn_sites();
    for (size_t i = 0; i < names.size(); i++) {
        players[names[i]] = new Player(coordinates[i], i + 1, names[i], obs);
        player_points[names[i]] = 0;
    }
    return players;
}

void Game::run() {
    const PlayerObserver* player_obs = new PlayerObserver(senders, ids);

    while (!game_over) {
        for (int i = 0; i < Config::get_instance()->rounds_per_cycle; i++) {

            Map& map = map_manager.get_random_map();
            current_stage = new Stage(map, senders, ids);
            send_map(map);

            if (!players_created) {
                generate_players(player_names, *player_obs, map);
                players_created = true;
            }
            game_loop = std::make_shared<GameLoop>(recv, players, is_testing, is_cheating);

            std::string winner = game_loop->play_round(*current_stage, map);
            player_points[winner] = player_points[winner] + 1;

            std::shared_ptr<GenericMsg> msg = std::make_shared<WinnerMsg>(winner);
            for (auto id: *ids) {
                senders.send_to_client(msg, id);
            }

            delete current_stage;
            current_stage = nullptr;
        }
        for (auto& [name, points]: player_points) {
            if (points >= 7) {
                std::cout << "Player " << name << " won the game!" << std::endl;
                game_over = true;
            }
        }
    }
    std::shared_ptr<GenericMsg> msg = std::make_shared<GameEndedMsg>();
    for (auto id: *ids) {
        senders.send_to_client(msg, id);
    }
    for (auto& player: players) {
        delete player.second;
    }
    players.clear();

    delete player_obs;
}

void Game::send_map(Map& map) {
    std::vector<uint16_t> map_vector = current_stage->get_vector_representation();
    std::shared_ptr<SendMapMsg> msg = std::make_shared<SendMapMsg>(
            map_vector, map.get_rows(), map.get_columns(), map.get_theme());
    for (auto id: *ids) {
        senders.send_to_client(msg, id);
    }
}

Game::~Game() {
    for (auto& player: players) {
        delete player.second;
    }
    players.clear();
}
