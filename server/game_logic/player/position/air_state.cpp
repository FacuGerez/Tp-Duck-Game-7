
#include "air_state.h"

#include <memory>

#include "../../../../common/duckState/duckState.h"
#include "../../config/config.h"
#include "../observer.h"

#include "player_position.h"

using namespace PlayerInfoId;
using namespace statesId;

// ------------------ Grounded ------------------

void Grounded::jump(PlayerPosition& player) {
    if (stopped_jumping) {
        player.set_state(std::make_unique<Jumping>(), JUMPING);
    }
}

int Grounded::get_offset() { return 0; }

void Grounded::update(bool could_fall, PlayerPosition& player) {
    if (could_fall) {
        player.set_state(std::make_unique<Falling>(), FALLING);
    }
}
void Grounded::stop_jumping(PlayerPosition&) { stopped_jumping = true; }

// ------------------ Jumping ------------------

void Jumping::stop_jumping(PlayerPosition& player) {
    player.set_state(std::make_unique<Falling>(), FALLING);
}

void Jumping::jump(PlayerPosition&) { keeps_jumping = true; }

void Jumping::update(bool could_fall, PlayerPosition& player) {
    if (!keeps_jumping) {
        jumps_left = 0;
    }
    jumps_left--;
    if (jumps_left <= 0) {
        if (could_fall) {
            player.set_state(std::make_unique<Falling>(), FALLING);
            return;
        }
        player.set_state(std::make_unique<Grounded>(!keeps_jumping), GROUNDED);
        return;
    }
    keeps_jumping = false;
}

int Jumping::get_offset() { return -2; }

// ------------------ Falling ------------------

int Falling::get_offset() { return falling_speed; }

void Falling::jump(PlayerPosition& player) {
    if (stopped_jumping) {
        falling_speed = 0;
        stopped_jumping = false;
        player.set_state(nullptr, SLOW_FALL);
    }
}

void Falling::update(bool could_fall, PlayerPosition& player) {
    if (!could_fall) {
        player.set_state(std::make_unique<Grounded>(stopped_jumping), GROUNDED);
        return;
    }
    falling_speed = Config::get_instance()->player_falling_speed;
}

void Falling::stop_jumping(PlayerPosition& player) { stopped_jumping = true; }

// ------------------ PlayingDead ------------------

int PlayingDead::get_offset() { return 0; }

void PlayingDead::jump(PlayerPosition& player) {
    player.set_state(std::make_unique<Jumping>(), JUMPING);
}

void PlayingDead::update(bool could_fall, PlayerPosition& player) {
    if (could_fall) {
        player.set_state(std::make_unique<Falling>(), FALLING);
    }
}

int Tripping::get_offset() { return falling_speed; }

void Tripping::jump(PlayerPosition& player) {}

void Tripping::update(bool could_fall, PlayerPosition& player) {
    if (could_fall) {
        falling_speed = Config::get_instance()->player_falling_speed;
    } else {
        falling_speed = 0;
    }
    turns_stunned++;
    if (turns_stunned == stun_duration) {
        if (could_fall) {
            player.set_state(std::make_unique<Falling>(), FALLING);
            return;
        }
        player.set_state(std::make_unique<Grounded>(true), GROUNDED);
    }
}
