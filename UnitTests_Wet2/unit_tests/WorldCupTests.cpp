#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "wet2util_override.h"
#include "../worldcup23a2.h"
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdlib.h>

using namespace std;

TEST_CASE("insert and remove team")
{
    SECTION("simple add and remove")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->remove_team(1);
        REQUIRE(res == StatusType::SUCCESS);

        delete obj;
    }

    SECTION("invalid input")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(0);
        REQUIRE(res == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("add existing")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(1);
        REQUIRE(res == StatusType::FAILURE);

        delete obj;
    }

    SECTION("remove then add")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->remove_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);

        delete obj;
    }

    SECTION("add and remove a lot")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res;
        for (int i = 1; i < 100; i++)
        {
            res = obj->add_team(i);
            REQUIRE(res == StatusType::SUCCESS);
        }
        for (int i = 1; i < 100; i++)
        {
            res = obj->remove_team(i);
            REQUIRE(res == StatusType::SUCCESS);
        }
        for (int i = 1; i < 100; i++)
        {
            res = obj->add_team(i);
            REQUIRE(res == StatusType::SUCCESS);
        }

        delete obj;
    }

    SECTION("add negative id")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(-1);
        REQUIRE(res == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("remove team that doesnt exist")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->remove_team(5);
        REQUIRE(res == StatusType::FAILURE);

        delete obj;
    }

    SECTION("remove team that doesnt exist but another team exists")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->remove_team(5);
        REQUIRE(res == StatusType::FAILURE);

        delete obj;
    }

    SECTION("remove negative id but another team exists")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(5);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->remove_team(-4);
        REQUIRE(res == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("remove 0 id but another team exists")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(5);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->remove_team(0);
        REQUIRE(res == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("remove negative id no team exists")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->remove_team(-4);
        REQUIRE(res == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("remove 0 id no team exists")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->remove_team(0);
        REQUIRE(res == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("remove team twice")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->remove_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->remove_team(1);
        REQUIRE(res == StatusType::FAILURE);

        delete obj;
    }
}

TEST_CASE("Add player")
{
    SECTION("simple add player")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 1, permutation_t::neutral(), 3, 3, 3, false);
        REQUIRE(res == StatusType::SUCCESS);

        delete obj;
    }

    SECTION("simple add goal keeper player")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 2, permutation_t::neutral(), 3, 3, 3, true);
        REQUIRE(res == StatusType::SUCCESS);

        delete obj;
    }

    SECTION("add player with negative id")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(-1, 2, permutation_t::neutral(), 3, 3, 3, true);
        REQUIRE(res == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("add player with negative team id")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, -2, permutation_t::neutral(), 3, 3, 3, true);
        REQUIRE(res == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("add player with invalid spirit")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 2, permutation_t::invalid(), 3, 3, 3, true);
        REQUIRE(res == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("add player with negative games")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 2, permutation_t::neutral(), -3, 3, 3, true);
        REQUIRE(res == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("add player with 0 id")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(0, 2, permutation_t::neutral(), 3, 3, 3, true);
        REQUIRE(res == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("add player with 0 team id")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 0, permutation_t::neutral(), 3, 3, 3, true);
        REQUIRE(res == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("add player with negative cards")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 2, permutation_t::neutral(), 3, 3, -3, true);
        REQUIRE(res == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("add player with negative ability")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 1, permutation_t::neutral(), 0, -5, 0, true);
        REQUIRE(res == StatusType::SUCCESS);

        delete obj;
    }

    SECTION("add player with 0 ability")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 1, permutation_t::neutral(), 0, 0, 3, true);
        REQUIRE(res == StatusType::SUCCESS);

        delete obj;
    }

    SECTION("add player with 0 id and 0 team id")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(0, 0, permutation_t::neutral(), 3, 3, 3, true);
        REQUIRE(res == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("add player with many zeroes")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(0, 2, permutation_t::neutral(), 0, 0, 0, true);
        REQUIRE(res == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("add player with zero all")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(0, 0, permutation_t::neutral(), 0, 0, 0, true);
        REQUIRE(res == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("add player that exists")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 1, permutation_t::neutral(), 3, 3, 3, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 1, permutation_t::neutral(), 3, 3, 3, true);
        REQUIRE(res == StatusType::FAILURE);

        delete obj;
    }

    SECTION("add player with team that doesn't exist")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 1, permutation_t::neutral(), 3, 3, 3, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(4, 4, permutation_t::neutral(), 4, 4, 4, true);
        REQUIRE(res == StatusType::FAILURE);

        delete obj;
    }

    SECTION("add player to two different teams")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 1, permutation_t::neutral(), 3, 3, 3, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 2, permutation_t::neutral(), 3, 3, 3, true);
        REQUIRE(res == StatusType::FAILURE);

        delete obj;
    }

    SECTION("add existing player that has no team")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 1, permutation_t::neutral(), 3, 3, 3, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->remove_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 2, permutation_t::neutral(), 3, 3, 3, true);
        REQUIRE(res == StatusType::FAILURE);

        delete obj;
    }

    SECTION("add player to removed team")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->remove_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 1, permutation_t::neutral(), 3, 3, 3, true);
        REQUIRE(res == StatusType::FAILURE);

        delete obj;
    }

    SECTION("add teams and players - segel test from wet1")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(3);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(4);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1001, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1002, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1003, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1004, 1, permutation_t::neutral(), 10, 0, 2, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1005, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1006, 1, permutation_t::neutral(), 10, 4, 3, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1007, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1008, 1, permutation_t::neutral(), 10, 0, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1009, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1010, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1011, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1012, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->add_player(2001, 2, permutation_t::neutral(), 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2002, 2, permutation_t::neutral(), 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2003, 2, permutation_t::neutral(), 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2004, 2, permutation_t::neutral(), 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2005, 2, permutation_t::neutral(), 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2006, 2, permutation_t::neutral(), 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2007, 2, permutation_t::neutral(), 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2008, 2, permutation_t::neutral(), 20, 6, 4, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2009, 2, permutation_t::neutral(), 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2010, 2, permutation_t::neutral(), 0, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2011, 2, permutation_t::neutral(), 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->add_player(3001, 3, permutation_t::neutral(), 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3002, 3, permutation_t::neutral(), 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3003, 3, permutation_t::neutral(), 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3004, 3, permutation_t::neutral(), 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3005, 3, permutation_t::neutral(), 30, 2, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3006, 3, permutation_t::neutral(), 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3007, 3, permutation_t::neutral(), 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3008, 3, permutation_t::neutral(), 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3009, 3, permutation_t::neutral(), 30, 0, 2, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3010, 3, permutation_t::neutral(), 30, 0, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3011, 3, permutation_t::neutral(), 30, 0, 0, true);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->add_player(4001, 4, permutation_t::neutral(), 2, 1, 2, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(4002, 4, permutation_t::neutral(), 2, 2, 2, false);
        REQUIRE(res == StatusType::SUCCESS);

        delete obj;
    }

    SECTION("add teams and players with random team remove")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(3);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(4);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1001, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1002, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1003, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1004, 1, permutation_t::neutral(), 10, 0, 2, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1005, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1006, 1, permutation_t::neutral(), 10, 4, 3, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1007, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1008, 1, permutation_t::neutral(), 10, 0, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1009, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1010, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1011, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1012, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->add_player(2001, 2, permutation_t::neutral(), 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2002, 2, permutation_t::neutral(), 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2003, 2, permutation_t::neutral(), 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2004, 2, permutation_t::neutral(), 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2005, 2, permutation_t::neutral(), 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2006, 2, permutation_t::neutral(), 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2007, 2, permutation_t::neutral(), 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2008, 2, permutation_t::neutral(), 20, 6, 4, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2009, 2, permutation_t::neutral(), 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->remove_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2010, 2, permutation_t::neutral(), 0, 0, 0, false);
        REQUIRE(res == StatusType::FAILURE);
        res = obj->add_player(2011, 2, permutation_t::neutral(), 20, 0, 0, false);
        REQUIRE(res == StatusType::FAILURE);

        res = obj->add_player(3001, 3, permutation_t::neutral(), 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3002, 3, permutation_t::neutral(), 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3003, 3, permutation_t::neutral(), 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3004, 3, permutation_t::neutral(), 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3005, 3, permutation_t::neutral(), 30, 2, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3006, 3, permutation_t::neutral(), 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3007, 3, permutation_t::neutral(), 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3008, 3, permutation_t::neutral(), 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3009, 3, permutation_t::neutral(), 30, 0, 2, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3010, 3, permutation_t::neutral(), 30, 0, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3011, 3, permutation_t::neutral(), 30, 0, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->remove_team(3);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->add_player(4001, 4, permutation_t::neutral(), 2, 1, 2, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(4002, 4, permutation_t::neutral(), 2, 2, 2, false);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->remove_team(4);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->add_player(4001, 4, permutation_t::neutral(), 2, 1, 2, false);
        REQUIRE(res == StatusType::FAILURE);


        delete obj;
    }

    SECTION("add teams and players with random team remove")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1001, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1002, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1003, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1004, 1, permutation_t::neutral(), 10, 0, 2, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1005, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->remove_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1006, 1, permutation_t::neutral(), 10, 4, 3, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->remove_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1007, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::FAILURE);
        res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1008, 1, permutation_t::neutral(), 10, 0, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1009, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1010, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1011, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1012, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);

        delete obj;
    }
}

TEST_CASE("play_match")
{
    SECTION("simple play_match")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);

        // first team players
        res = obj->add_player(1001, 1, permutation_t::neutral(), 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1002, 1, permutation_t::neutral(), 1, 50000, 1, true);
        REQUIRE(res == StatusType::SUCCESS);


        // second team players
        res = obj->add_player(2001, 2, permutation_t::neutral(), 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2002, 2, permutation_t::neutral(), 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2005, 2, permutation_t::neutral(), 1, 1, 1, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2011, 2, permutation_t::neutral(), 1, 1, 1, true);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res1 = obj->play_match(1, 2);
        REQUIRE(res1.status() == StatusType::SUCCESS);
        REQUIRE(res1.ans() == 1);

        delete obj;
    }

    SECTION("play_match invalid input")
    {
        world_cup_t* obj = new world_cup_t();

        output_t<int> res1 = obj->play_match(0, 1);
        REQUIRE(res1.status() == StatusType::INVALID_INPUT);
        output_t<int> res2 = obj->play_match(1, 0);
        REQUIRE(res2.status() == StatusType::INVALID_INPUT);
        output_t<int> res3 = obj->play_match(0, 0);
        REQUIRE(res3.status() == StatusType::INVALID_INPUT);
        output_t<int> res4 = obj->play_match(3, 3);
        REQUIRE(res4.status() == StatusType::INVALID_INPUT);
        output_t<int> res5 = obj->play_match(-1, 1);
        REQUIRE(res5.status() == StatusType::INVALID_INPUT);
        output_t<int> res6 = obj->play_match(1, -1);
        REQUIRE(res6.status() == StatusType::INVALID_INPUT);
        output_t<int> res7 = obj->play_match(-1, -1);
        REQUIRE(res7.status() == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("play_match no goal keepers")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);

        // first team players
        res = obj->add_player(1001, 1, permutation_t::neutral(), 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1002, 1, permutation_t::neutral(), 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);

        //second team players
        res = obj->add_player(2001, 2, permutation_t::neutral(), 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2002, 2, permutation_t::neutral(), 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2003, 2, permutation_t::neutral(), 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);


        output_t<int> res1 = obj->play_match(1, 2);
        REQUIRE(res1.status() == StatusType::FAILURE);

        delete obj;
    }

    SECTION("play_match goal keepers only on one")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);

        // first team players
        res = obj->add_player(1005, 1, permutation_t::neutral(), 1, 1, 1, true);
        REQUIRE(res == StatusType::SUCCESS);

        // second team players
        res = obj->add_player(2001, 2, permutation_t::neutral(), 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);


        output_t<int> res1 = obj->play_match(1, 2);
        REQUIRE(res1.status() == StatusType::FAILURE);

        delete obj;
    }


    SECTION("play_match extra failure")
    {
        world_cup_t* obj = new world_cup_t();

        output_t<int> res1 = obj->play_match(1, 2);
        REQUIRE(res1.status() == StatusType::FAILURE);
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> res2 = obj->play_match(2, 1);
        REQUIRE(res2.status() == StatusType::FAILURE);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> res3 = obj->play_match(1, 2);
        REQUIRE(res3.status() == StatusType::FAILURE);
        res = obj->add_player(1001, 1, permutation_t::neutral(), 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1002, 1, permutation_t::neutral(), 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2001, 2, permutation_t::neutral(), 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2002, 2, permutation_t::neutral(), 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> res4 = obj->play_match(1, 2);
        REQUIRE(res4.status() == StatusType::FAILURE);
        res = obj->add_player(1003, 1, permutation_t::neutral(), 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2003, 2, permutation_t::neutral(), 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> res5 = obj->play_match(1, 2);
        REQUIRE(res5.status() == StatusType::FAILURE);
        res = obj->add_player(1004, 1, permutation_t::neutral(), 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1005, 1, permutation_t::neutral(), 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1006, 1, permutation_t::neutral(), 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2004, 2, permutation_t::neutral(), 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2005, 2, permutation_t::neutral(), 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2006, 2, permutation_t::neutral(), 1, 1, 1, true);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> res6 = obj->play_match(1, 2);
        REQUIRE(res6.status() == StatusType::FAILURE);
        res = obj->add_player(1007, 1, permutation_t::neutral(), 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1008, 1, permutation_t::neutral(), 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1009, 1, permutation_t::neutral(), 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1010, 1, permutation_t::neutral(), 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1011, 1, permutation_t::neutral(), 1, 1, 1, true);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> res7 = obj->play_match(1, 2);
        REQUIRE(res7.status() == StatusType::SUCCESS);
        res = obj->remove_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> res8 = obj->play_match(1, 2);
        REQUIRE(res8.status() == StatusType::FAILURE);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> res9 = obj->play_match(1, 2);
        REQUIRE(res9.status() == StatusType::FAILURE);
        res = obj->add_player(2007, 2, permutation_t::neutral(), 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> res9p = obj->play_match(1, 2);
        REQUIRE(res9p.status() == StatusType::FAILURE);
        delete obj;
    }

    SECTION("play_match success returns")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->add_player(1001, 1, permutation_t::neutral(), 1, 5, 1, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2001, 2, permutation_t::neutral(), 1, 1, 1, true);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res1 = obj->play_match(1, 2);
        REQUIRE(res1.status() == StatusType::SUCCESS);
        REQUIRE(res1.ans() == 1); // 5|1
        // score now 3:0
        res = obj->add_player(2002, 2, permutation_t::neutral(), 1, 10, 1, true);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res2 = obj->play_match(1, 2);
        REQUIRE(res2.status() == StatusType::SUCCESS);
        REQUIRE(res2.ans() == 3); // 8|11
        //score now 3:3

        res = obj->add_player(1002, 1, permutation_t::neutral(), 1, 6, 1, true);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res3 = obj->play_match(1, 2);
        REQUIRE(res3.status() == StatusType::SUCCESS);
        REQUIRE(res3.ans() == 0);// 14|14
        //score now 4:4

        res = obj->add_player(1003, 1, permutation_t::neutral(), 1, 5, 1, true);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res4 = obj->play_match(2, 1);
        REQUIRE(res4.status() == StatusType::SUCCESS);
        REQUIRE(res4.ans() == 3);// 20|15
        //score now 23:15
        res = obj->add_player(2003, 2, permutation_t::neutral(), 1, 10, 1, true);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res5 = obj->play_match(2, 1); //
        REQUIRE(res5.status() == StatusType::SUCCESS);
        REQUIRE(res5.ans() == 1);// 23|25

        res = obj->add_player(1004, 1, permutation_t::neutral(), 1, 5, 1, true);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res6 = obj->play_match(2, 1);
        REQUIRE(res6.status() == StatusType::SUCCESS);
        REQUIRE(res6.ans() == 0);// 25|25

        res = obj->add_player(1005, 1, permutation_t::read("2,3,4,5,1"), 1, 0, 1, true);
        REQUIRE(res == StatusType::SUCCESS); 

        output_t<int> res7 = obj->play_match(1, 2);
        REQUIRE(res7.status() == StatusType::SUCCESS);
        REQUIRE(res7.ans() == 4);// 26|26 -> str[45|55]

        res = obj->add_player(1104, 1, permutation_t::neutral(), 1, 3, 1, true);
        res = obj->add_player(2004, 2, permutation_t::read("4,3,5,2,1"), 1, 0, 1, true);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res8 = obj->play_match(1, 2);
        REQUIRE(res8.status() == StatusType::SUCCESS);
        REQUIRE(res8.ans() == 2);// 29|29 -> str[45|38]

        res = obj->add_player(1300, 1, permutation_t::read("5,1,2,3,4"), 1, 0, 1, true); //to neutral
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1006, 1, permutation_t::read("4,3,5,2,1"), 1, 3, 1, true);
        REQUIRE(res == StatusType::SUCCESS); 
        res = obj->add_player(2300, 2, permutation_t::read("1,2,3,4,5"), 1, 6, 1, true);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> res9 = obj->play_match(2, 1);
        REQUIRE(res9.status() == StatusType::SUCCESS);
        REQUIRE(res9.ans() == 0);// 32|32 -> str[38|38]

        res = obj->add_player(2005, 2, permutation_t::read("2,3,4,5,1"), 1, 0, 1, true);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res10 = obj->play_match(2, 1);
        REQUIRE(res10.status() == StatusType::SUCCESS);
        REQUIRE(res10.ans() == 2);// 33|33 -> str[43|38]

        res = obj->add_player(1007, 1, permutation_t::read("5,4,1,3,2").inv(), 1, 3, 1, true);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res11 = obj->play_match(2, 1);
        REQUIRE(res11.status() == StatusType::SUCCESS);
        REQUIRE(res11.ans() == 4);// 36|36 -> str[43|55]


        delete obj;
    }
}


TEST_CASE("num_played_games_for_player")
{
    SECTION("simple get played")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1001, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1002, 1, permutation_t::neutral(), 0, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> res1 = obj->num_played_games_for_player(1001);
        REQUIRE(res1.status() == StatusType::SUCCESS);
        REQUIRE(res1.ans() == 10);

        delete obj;
    }

    SECTION("simple get played 2")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1001, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1002, 1, permutation_t::neutral(), 0, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> res1 = obj->num_played_games_for_player(1002);
        REQUIRE(res1.status() == StatusType::SUCCESS);
        REQUIRE(res1.ans() == 0);

        delete obj;
    }

    SECTION("get num played for non exist player")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1001, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1002, 1, permutation_t::neutral(), 0, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> res1 = obj->num_played_games_for_player(1003);
        REQUIRE(res1.status() == StatusType::FAILURE);

        delete obj;
    }

    SECTION("get num played for non exist player2")
    {
        world_cup_t* obj = new world_cup_t();
        output_t<int> res1 = obj->num_played_games_for_player(1003);
        REQUIRE(res1.status() == StatusType::FAILURE);

        delete obj;
    }

    SECTION("get_num_played for negative player id")
    {
        world_cup_t* obj = new world_cup_t();
        output_t<int> res1 = obj->num_played_games_for_player(-1);
        REQUIRE(res1.status() == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("get_num_played for 0 player id")
    {
        world_cup_t* obj = new world_cup_t();
        output_t<int> res1 = obj->num_played_games_for_player(0);
        REQUIRE(res1.status() == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("segel test from wet1")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(3);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(4);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1001, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1002, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res1 = obj->num_played_games_for_player(1002);
        REQUIRE(res1.status() == StatusType::SUCCESS);
        REQUIRE(res1.ans() == 10);

        res = obj->add_player(1003, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1004, 1, permutation_t::neutral(), 10, 0, 2, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1005, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1006, 1, permutation_t::neutral(), 10, 4, 3, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1007, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res2 = obj->num_played_games_for_player(1006);
        REQUIRE(res2.status() == StatusType::SUCCESS);
        REQUIRE(res2.ans() == 10);

        res = obj->add_player(1008, 1, permutation_t::neutral(), 10, 0, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1009, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1010, 1, permutation_t::neutral(), 10, 0, 0, false);

        output_t<int> res3 = obj->num_played_games_for_player(1010);
        REQUIRE(res3.status() == StatusType::SUCCESS);
        REQUIRE(res3.ans() == 10);

        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1011, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1012, 1, permutation_t::neutral(), 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->add_player(2001, 2, permutation_t::neutral(), 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2002, 2, permutation_t::neutral(), 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2003, 2, permutation_t::neutral(), 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2004, 2, permutation_t::neutral(), 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2005, 2, permutation_t::neutral(), 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2006, 2, permutation_t::neutral(), 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2007, 2, permutation_t::neutral(), 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2008, 2, permutation_t::neutral(), 20, 6, 4, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2009, 2, permutation_t::neutral(), 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2010, 2, permutation_t::neutral(), 0, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2011, 2, permutation_t::neutral(), 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->add_player(3001, 3, permutation_t::neutral(), 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3002, 3, permutation_t::neutral(), 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3003, 3, permutation_t::neutral(), 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3004, 3, permutation_t::neutral(), 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3005, 3, permutation_t::neutral(), 30, 2, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3006, 3, permutation_t::neutral(), 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3007, 3, permutation_t::neutral(), 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3008, 3, permutation_t::neutral(), 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3009, 3, permutation_t::neutral(), 30, 0, 2, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3010, 3, permutation_t::neutral(), 30, 0, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3011, 3, permutation_t::neutral(), 30, 0, 0, true);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->add_player(4001, 4, permutation_t::neutral(), 2, 1, 2, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(4002, 4, permutation_t::neutral(), 2, 2, 2, false);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res4 = obj->num_played_games_for_player(3001);
        REQUIRE(res4.status() == StatusType::SUCCESS);
        REQUIRE(res4.ans() == 30);

        output_t<int> res5 = obj->num_played_games_for_player(3011);
        REQUIRE(res5.status() == StatusType::SUCCESS);
        REQUIRE(res5.ans() == 30);

        output_t<int> res6 = obj->num_played_games_for_player(1001);
        REQUIRE(res6.status() == StatusType::SUCCESS);
        REQUIRE(res6.ans() == 10);

        output_t<int> res7 = obj->num_played_games_for_player(4001);
        REQUIRE(res7.status() == StatusType::SUCCESS);
        REQUIRE(res7.ans() == 2);


        delete obj;
    }

    SECTION("simple get_num_played after a match")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1001, 1, permutation_t::neutral(), 1, 0, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2001, 2, permutation_t::neutral(), 0, 0, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> res1 = obj->num_played_games_for_player(1001);
        REQUIRE(res1.status() == StatusType::SUCCESS);
        REQUIRE(res1.ans() == 1);
        output_t<int> res2 = obj->num_played_games_for_player(2001);
        REQUIRE(res2.status() == StatusType::SUCCESS);
        REQUIRE(res2.ans() == 0);
        output_t<int> res3 = obj->play_match(1, 2);
        REQUIRE(res3.status() == StatusType::SUCCESS);
        REQUIRE(res3.ans() == 0);
        output_t<int> res4 = obj->num_played_games_for_player(1001);
        REQUIRE(res4.status() == StatusType::SUCCESS);
        REQUIRE(res4.ans() == 2);
        output_t<int> res5 = obj->num_played_games_for_player(2001);
        REQUIRE(res5.status() == StatusType::SUCCESS);
        REQUIRE(res5.ans() == 1);


        delete obj;
    }

    SECTION("complex get_num_played after a match")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1001, 1, permutation_t::neutral(), 1, 0, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1002, 1, permutation_t::neutral(), 2, 0, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1003, 1, permutation_t::neutral(), 3, 0, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1004, 1, permutation_t::neutral(), 4, 0, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2001, 2, permutation_t::neutral(), 1, 0, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2002, 2, permutation_t::neutral(), 0, 0, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> res1 = obj->num_played_games_for_player(1001);
        REQUIRE(res1.status() == StatusType::SUCCESS);
        REQUIRE(res1.ans() == 1);
        output_t<int> res11 = obj->num_played_games_for_player(1002);
        REQUIRE(res11.status() == StatusType::SUCCESS);
        REQUIRE(res11.ans() == 2);
        output_t<int> res12 = obj->num_played_games_for_player(1003);
        REQUIRE(res12.status() == StatusType::SUCCESS);
        REQUIRE(res12.ans() == 3);
        output_t<int> res13 = obj->num_played_games_for_player(1004);
        REQUIRE(res13.status() == StatusType::SUCCESS);
        REQUIRE(res13.ans() == 4);
        output_t<int> res14 = obj->num_played_games_for_player(2001);
        REQUIRE(res14.status() == StatusType::SUCCESS);
        REQUIRE(res14.ans() == 1);
        output_t<int> res15 = obj->num_played_games_for_player(2002);
        REQUIRE(res15.status() == StatusType::SUCCESS);
        REQUIRE(res15.ans() == 0);
        output_t<int> res3 = obj->play_match(1, 2);
        REQUIRE(res3.status() == StatusType::SUCCESS);
        REQUIRE(res3.ans() == 0);
        output_t<int> res2 = obj->num_played_games_for_player(1001);
        REQUIRE(res2.status() == StatusType::SUCCESS);
        REQUIRE(res2.ans() == 2);
        output_t<int> res21 = obj->num_played_games_for_player(1002);
        REQUIRE(res21.status() == StatusType::SUCCESS);
        REQUIRE(res21.ans() == 3);
        output_t<int> res22 = obj->num_played_games_for_player(1003);
        REQUIRE(res22.status() == StatusType::SUCCESS);
        REQUIRE(res22.ans() == 4);
        output_t<int> res23 = obj->num_played_games_for_player(1004);
        REQUIRE(res23.status() == StatusType::SUCCESS);
        REQUIRE(res23.ans() == 5);
        output_t<int> res24 = obj->num_played_games_for_player(2001);
        REQUIRE(res24.status() == StatusType::SUCCESS);
        REQUIRE(res24.ans() == 2);
        output_t<int> res25 = obj->num_played_games_for_player(2002);
        REQUIRE(res25.status() == StatusType::SUCCESS);
        REQUIRE(res25.ans() == 1);


        delete obj;
    }

}


TEST_CASE("add and get cards")
{
    SECTION("simple add cards")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 1, permutation_t::neutral(), 1, 1, 1, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player_cards(1, 1);
        REQUIRE(res == StatusType::SUCCESS);

        delete obj;
    }

    SECTION("add cards bad id")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 1, permutation_t::neutral(), 1, 1, 1, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player_cards(-1, 1);
        REQUIRE(res == StatusType::INVALID_INPUT);
        res = obj->add_player_cards(0, 1);
        REQUIRE(res == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("add cards bad cards")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 1, permutation_t::neutral(), 1, 1, 1, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player_cards(1, -1);
        REQUIRE(res == StatusType::INVALID_INPUT);
        res = obj->add_player_cards(1, 0);
        REQUIRE(res == StatusType::SUCCESS);

        delete obj;
    }

    SECTION("add cards bad cards and id")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 1, permutation_t::neutral(), 1, 1, 1, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player_cards(-1, -1);
        REQUIRE(res == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("add cards player never existed")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2, 1, permutation_t::neutral(), 1, 1, 1, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player_cards(1, 1);
        REQUIRE(res == StatusType::FAILURE);

        delete obj;
    }

    SECTION("add cards player never existed +invalid input")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2, 1, permutation_t::neutral(), 1, 1, 1, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player_cards(-1, 1);
        REQUIRE(res == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("add cards player taken out of tournament")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 1, permutation_t::neutral(), 1, 1, 1, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->remove_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player_cards(1, 1);
        REQUIRE(res == StatusType::FAILURE);

        delete obj;
    }

    SECTION("add cards player taken out of tournament + bad input")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 1, permutation_t::neutral(), 1, 1, 1, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->remove_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player_cards(1, -1);
        REQUIRE(res == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("simple get cards")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 1, permutation_t::neutral(), 1, 1, 1, true);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> resn3 = obj->get_player_cards(1);
        REQUIRE(resn3.status() == StatusType::SUCCESS);
        REQUIRE(resn3.ans() == 1);

        delete obj;
    }

    SECTION("get cards bad id")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 1, permutation_t::neutral(), 1, 1, 1, true);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> resn3 = obj->get_player_cards(-1);
        REQUIRE(resn3.status() == StatusType::INVALID_INPUT);
        output_t<int> resn4 = obj->get_player_cards(0);
        REQUIRE(resn4.status() == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("get cards player never existed")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2, 1, permutation_t::neutral(), 1, 1, 1, true);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> resn4 = obj->get_player_cards(1);
        REQUIRE(resn4.status() == StatusType::FAILURE);

        delete obj;
    }

    SECTION("add cards player never existed +invalid input")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2, 1, permutation_t::neutral(), 1, 1, 1, true);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> resn4 = obj->get_player_cards(-1);
        REQUIRE(resn4.status() == StatusType::INVALID_INPUT);
        output_t<int> resn1 = obj->get_player_cards(0);
        REQUIRE(resn1.status() == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("get cards player taken out of tournament")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 1, permutation_t::neutral(), 1, 1, 1, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->remove_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> resn4 = obj->get_player_cards(1);
        REQUIRE(resn4.status() == StatusType::SUCCESS);
        REQUIRE(resn4.ans() == 1);

        delete obj;
    }

    SECTION("add cards player taken out of tournament + bad input")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 1, permutation_t::neutral(), 1, 1, 1, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->remove_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> resn4 = obj->get_player_cards(0);
        REQUIRE(resn4.status() == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("get cards player taken out of tournament but add beforehand")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 1, permutation_t::neutral(), 1, 1, 1, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player_cards(1, 1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->remove_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> resn4 = obj->get_player_cards(1);
        REQUIRE(resn4.status() == StatusType::SUCCESS);
        REQUIRE(resn4.ans() == 2);

        delete obj;
    }

    SECTION("get cards after recurring addition and kicking player")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 1, permutation_t::neutral(), 1, 1, 1, true);
        REQUIRE(res == StatusType::SUCCESS);
        for (int i = 1; i <= 99; ++i)
        {
            res = obj->add_player_cards(1, 1);
            REQUIRE(res == StatusType::SUCCESS);
            output_t<int> resni = obj->get_player_cards(1);
            REQUIRE(resni.status() == StatusType::SUCCESS);
            REQUIRE(resni.ans() == 1 + i);

        }
        res = obj->remove_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> resn4 = obj->get_player_cards(1);
        REQUIRE(resn4.status() == StatusType::SUCCESS);
        REQUIRE(resn4.ans() == 100);

        delete obj;
    }
}

TEST_CASE("get team points")
{
    SECTION("simple get points for new team")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> resn4 = obj->get_team_points(1);
        REQUIRE(resn4.status() == StatusType::SUCCESS);
        REQUIRE(resn4.ans() == 0);

        delete obj;
    }

    SECTION("simple get points after match")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        for (int i = 1; i <= 20; ++i)
        {
            StatusType res1 = obj->add_player(i+1000, 1, permutation_t::neutral(), 1, 1, 1, true);;
            REQUIRE(res1 == StatusType::SUCCESS);

        }
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        for (int i = 1; i <= 30; ++i)
        {
            StatusType res1 = obj->add_player(i+2000, 2, permutation_t::neutral(), 1, 1, 1, true);;
            REQUIRE(res1 == StatusType::SUCCESS);
        }
        output_t<int> resn02 = obj->get_team_points(2);
        REQUIRE(resn02.status() == StatusType::SUCCESS);
        REQUIRE(resn02.ans() == 0);
        output_t<int> resn01 = obj->get_team_points(2);
        REQUIRE(resn01.status() == StatusType::SUCCESS);
        REQUIRE(resn01.ans() == 0);
        output_t<int> resnP = obj->play_match(1, 2);
        REQUIRE(resnP.status() == StatusType::SUCCESS);
        REQUIRE(resnP.ans() == 3);
        output_t<int> resnpp2 = obj->get_team_points(2);
        REQUIRE(resnpp2.status() == StatusType::SUCCESS);
        REQUIRE(resnpp2.ans() == 3);
        output_t<int> resnpp1 = obj->get_team_points(1);
        REQUIRE(resnpp1.status() == StatusType::SUCCESS);
        REQUIRE(resnpp1.ans() == 0);

        delete obj;
    }

    SECTION("get points invalid input")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> resn4 = obj->get_team_points(-1);
        REQUIRE(resn4.status() == StatusType::INVALID_INPUT);
        output_t<int> resn5 = obj->get_team_points(0);
        REQUIRE(resn5.status() == StatusType::INVALID_INPUT);

        delete obj;
    }SECTION("get points no team with that id")
    {
        world_cup_t* obj = new world_cup_t();
        output_t<int> resn4 = obj->get_team_points(2);
        REQUIRE(resn4.status() == StatusType::FAILURE);
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> resn5 = obj->get_team_points(2);
        REQUIRE(resn5.status() == StatusType::FAILURE);

        delete obj;
    }

    SECTION("get points team with that id was removed")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> resn4 = obj->get_team_points(1);
        REQUIRE(resn4.status() == StatusType::SUCCESS);
        REQUIRE(resn4.ans() == 0);
        res = obj->remove_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> resn5 = obj->get_team_points(1);
        REQUIRE(resn5.status() == StatusType::FAILURE);

        delete obj;
    }

    SECTION("get points team with that id was removed then readded")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        for (int i = 1; i <= 30; ++i)
        {
            StatusType res1 = obj->add_player(1000+i, 1, permutation_t::neutral(), 1, 1, 1, true);;
            REQUIRE(res1 == StatusType::SUCCESS);

        }
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        for (int i = 1; i <= 10; ++i)
        {
            StatusType res1 = obj->add_player(2000+i, 2, permutation_t::neutral(), 1, 1, 1, true);;
            REQUIRE(res1 == StatusType::SUCCESS);
        }
        output_t<int> resnP = obj->play_match(1, 2);
        REQUIRE(resnP.status() == StatusType::SUCCESS);
        REQUIRE(resnP.ans() == 1);
        output_t<int> resn4 = obj->get_team_points(1);
        REQUIRE(resn4.status() == StatusType::SUCCESS);
        REQUIRE(resn4.ans() == 3);

        res = obj->remove_team(1);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> resn5 = obj->get_team_points(1);
        REQUIRE(resn5.status() == StatusType::FAILURE);

        res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> resn6 = obj->get_team_points(1);
        REQUIRE(resn6.status() == StatusType::SUCCESS);
        REQUIRE(resn6.ans() == 0);

        delete obj;
    }
}

TEST_CASE("get_ith_pointless_ability")
{
    SECTION("simple get i")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1001, 1, permutation_t::neutral(), 1, 4, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2001, 2, permutation_t::neutral(), 0, 14, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> res1 = obj->get_ith_pointless_ability(0);
        REQUIRE(res1.status() == StatusType::SUCCESS);
        REQUIRE(res1.ans() == 1);
        output_t<int> res2 = obj->get_ith_pointless_ability(1);
        REQUIRE(res2.status() == StatusType::SUCCESS);
        REQUIRE(res2.ans() == 2);

        delete obj;
    }

    SECTION("no teams")
    {
        world_cup_t* obj = new world_cup_t();

        output_t<int> res1 = obj->get_ith_pointless_ability(0);
        REQUIRE(res1.status() == StatusType::FAILURE);
        output_t<int> res2 = obj->get_ith_pointless_ability(5);
        REQUIRE(res2.status() == StatusType::FAILURE);
        output_t<int> res3 = obj->get_ith_pointless_ability(-5);
        REQUIRE(res3.status() == StatusType::FAILURE);

        delete obj;
    }

    SECTION("negative i")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> res1 = obj->get_ith_pointless_ability(-1);
        REQUIRE(res1.status() == StatusType::FAILURE);
        output_t<int> res2 = obj->get_ith_pointless_ability(-5);
        REQUIRE(res2.status() == StatusType::FAILURE);
        res = obj->add_player(1001, 1, permutation_t::neutral(), 1, 4, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2001, 2, permutation_t::neutral(), 0, 14, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> res3 = obj->get_ith_pointless_ability(-2);
        REQUIRE(res3.status() == StatusType::FAILURE);
        output_t<int> res4 = obj->get_ith_pointless_ability(-1);
        REQUIRE(res4.status() == StatusType::FAILURE);


        delete obj;
    }SECTION("i too big")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> res1 = obj->get_ith_pointless_ability(5);
        REQUIRE(res1.status() == StatusType::FAILURE);
        output_t<int> res2 = obj->get_ith_pointless_ability(107);
        REQUIRE(res2.status() == StatusType::FAILURE);
        res = obj->add_player(1001, 1, permutation_t::neutral(), 1, 4, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2001, 2, permutation_t::neutral(), 0, 14, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> res3 = obj->get_ith_pointless_ability(4);
        REQUIRE(res3.status() == StatusType::FAILURE);
        output_t<int> res4 = obj->get_ith_pointless_ability(2);
        REQUIRE(res4.status() == StatusType::FAILURE);
        res = obj->remove_team(1);
        output_t<int> res5 = obj->get_ith_pointless_ability(1);
        REQUIRE(res5.status() == StatusType::FAILURE);
        output_t<int> res6 = obj->get_ith_pointless_ability(2);
        REQUIRE(res6.status() == StatusType::FAILURE);


        delete obj;
    }

    SECTION("no players")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> res0 = obj->get_ith_pointless_ability(0);
        REQUIRE(res0.status() == StatusType::SUCCESS);
        REQUIRE(res0.ans() == 2);
        res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> res1 = obj->get_ith_pointless_ability(0);
        REQUIRE(res1.status() == StatusType::SUCCESS);
        REQUIRE(res1.ans() == 1);
        output_t<int> res2 = obj->get_ith_pointless_ability(1);
        REQUIRE(res2.status() == StatusType::SUCCESS);
        REQUIRE(res2.ans() == 2);
        res = obj->remove_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> res3 = obj->get_ith_pointless_ability(0);
        REQUIRE(res3.status() == StatusType::SUCCESS);
        REQUIRE(res3.ans() == 1);

        for (int i = 2; i < 52; i++)
        {
            res = obj->add_team(i);
            REQUIRE(res == StatusType::SUCCESS);
        }

        output_t<int> res4 = obj->get_ith_pointless_ability(15);
        REQUIRE(res4.status() == StatusType::SUCCESS);
        REQUIRE(res4.ans() == 16);
        output_t<int> res5 = obj->get_ith_pointless_ability(47);
        REQUIRE(res5.status() == StatusType::SUCCESS);
        REQUIRE(res5.ans() == 48);
        output_t<int> res6 = obj->get_ith_pointless_ability(1);
        REQUIRE(res6.status() == StatusType::SUCCESS);
        REQUIRE(res6.ans() == 2);
        output_t<int> res7 = obj->get_ith_pointless_ability(4);
        REQUIRE(res7.status() == StatusType::SUCCESS);
        REQUIRE(res7.ans() == 5);
        output_t<int> res8 = obj->get_ith_pointless_ability(39);
        REQUIRE(res8.status() == StatusType::SUCCESS);
        REQUIRE(res8.ans() == 40);
        output_t<int> res9 = obj->get_ith_pointless_ability(10);
        REQUIRE(res9.status() == StatusType::SUCCESS);
        REQUIRE(res9.ans() == 11);

        delete obj;
    }

    SECTION("with players")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(3);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(4);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->add_player(1001, 1, permutation_t::neutral(), 1, 4, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1002, 1, permutation_t::neutral(), 0, 14, 0, true);
        REQUIRE(res == StatusType::SUCCESS);


        output_t<int> res1 = obj->get_ith_pointless_ability(3);
        REQUIRE(res1.status() == StatusType::SUCCESS);
        REQUIRE(res1.ans() == 1);

        res = obj->add_player(2001, 2, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3001, 3, permutation_t::neutral(), 0, 5000, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(4001, 4, permutation_t::neutral(), 1, 1000, 0, true);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res2 = obj->get_ith_pointless_ability(0);
        REQUIRE(res2.status() == StatusType::SUCCESS);
        REQUIRE(res2.ans() == 2);
        output_t<int> res3 = obj->get_ith_pointless_ability(1);
        REQUIRE(res3.status() == StatusType::SUCCESS);
        REQUIRE(res3.ans() == 1);
        output_t<int> res4 = obj->get_ith_pointless_ability(2);
        REQUIRE(res4.status() == StatusType::SUCCESS);
        REQUIRE(res4.ans() == 4);
        output_t<int> res5 = obj->get_ith_pointless_ability(3);
        REQUIRE(res5.status() == StatusType::SUCCESS);
        REQUIRE(res5.ans() == 3);

        res = obj->add_player(2002, 2, permutation_t::neutral(), 1, 1000000, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3002, 3, permutation_t::neutral(), 0, -5000, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(4002, 4, permutation_t::neutral(), 1, -1000, 0, true);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res6 = obj->get_ith_pointless_ability(0);
        REQUIRE(res6.status() == StatusType::SUCCESS);
        REQUIRE(res6.ans() == 3);
        output_t<int> res7 = obj->get_ith_pointless_ability(1);
        REQUIRE(res7.status() == StatusType::SUCCESS);
        REQUIRE(res7.ans() == 4);
        output_t<int> res8 = obj->get_ith_pointless_ability(2);
        REQUIRE(res8.status() == StatusType::SUCCESS);
        REQUIRE(res8.ans() == 1);
        output_t<int> res9 = obj->get_ith_pointless_ability(3);
        REQUIRE(res9.status() == StatusType::SUCCESS);
        REQUIRE(res9.ans() == 2);

        delete obj;
    }

    SECTION("more points less ability")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(3);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(4);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->add_player(1001, 1, permutation_t::neutral(), 1, 6, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2001, 2, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3001, 3, permutation_t::neutral(), 0, 3, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(4001, 4, permutation_t::neutral(), 1, 2, 0, true);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res2 = obj->get_ith_pointless_ability(0);
        REQUIRE(res2.status() == StatusType::SUCCESS);
        REQUIRE(res2.ans() == 2);
        output_t<int> res3 = obj->get_ith_pointless_ability(1);
        REQUIRE(res3.status() == StatusType::SUCCESS);
        REQUIRE(res3.ans() == 4);
        output_t<int> res4 = obj->get_ith_pointless_ability(2);
        REQUIRE(res4.status() == StatusType::SUCCESS);
        REQUIRE(res4.ans() == 3);
        output_t<int> res5 = obj->get_ith_pointless_ability(3);
        REQUIRE(res5.status() == StatusType::SUCCESS);
        REQUIRE(res5.ans() == 1);

        output_t<int> res6 = obj->play_match(4, 2);
        REQUIRE(res6.status() == StatusType::SUCCESS);
        REQUIRE(res6.ans() == 1);
        output_t<int> res7 = obj->play_match(4, 2);
        REQUIRE(res7.status() == StatusType::SUCCESS);
        REQUIRE(res7.ans() == 1);
        output_t<int> res8 = obj->play_match(4, 2);
        REQUIRE(res8.status() == StatusType::SUCCESS);
        REQUIRE(res8.ans() == 1);
        output_t<int> res9 = obj->play_match(4, 2);
        REQUIRE(res9.status() == StatusType::SUCCESS);
        REQUIRE(res9.ans() == 1);
        output_t<int> res10 = obj->play_match(4, 2);
        REQUIRE(res10.status() == StatusType::SUCCESS);
        REQUIRE(res10.ans() == 1);
        output_t<int> res11 = obj->play_match(4, 2);
        REQUIRE(res11.status() == StatusType::SUCCESS);
        REQUIRE(res11.ans() == 1);

        output_t<int> res12 = obj->get_ith_pointless_ability(0);
        REQUIRE(res12.status() == StatusType::SUCCESS);
        REQUIRE(res12.ans() == 2);
        output_t<int> res13 = obj->get_ith_pointless_ability(1);
        REQUIRE(res13.status() == StatusType::SUCCESS);
        REQUIRE(res13.ans() == 4);
        output_t<int> res14 = obj->get_ith_pointless_ability(2);
        REQUIRE(res14.status() == StatusType::SUCCESS);
        REQUIRE(res14.ans() == 3);
        output_t<int> res15 = obj->get_ith_pointless_ability(3);
        REQUIRE(res15.status() == StatusType::SUCCESS);
        REQUIRE(res15.ans() == 1);

        delete obj;
    }


    SECTION("get ith but some teams are removed")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(5);
        REQUIRE(res == StatusType::SUCCESS);
        for (int i = 1; i <= 10; ++i)
        {
            StatusType res1 = obj->add_player(5000+i, 5, permutation_t::neutral(), 1, 1, 1, true);;
            REQUIRE(res1 == StatusType::SUCCESS);

        }
        res = obj->add_team(4);
        REQUIRE(res == StatusType::SUCCESS);
        for (int i = 1; i <= 20; ++i)
        {
            StatusType res1 = obj->add_player(4000+i, 4, permutation_t::neutral(), 1, 1, 1, true);;
            REQUIRE(res1 == StatusType::SUCCESS);
        }
        res = obj->add_team(3);
        REQUIRE(res == StatusType::SUCCESS);
        for (int i = 1; i <= 30; ++i)
        {
            StatusType res1 = obj->add_player(3000+i, 3, permutation_t::neutral(), 1, 1, 1, true);;
            REQUIRE(res1 == StatusType::SUCCESS);
        }
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        for (int i = 1; i <= 40; ++i)
        {
            StatusType res1 = obj->add_player(2000+i, 2, permutation_t::neutral(), 1, 1, 1, true);;
            REQUIRE(res1 == StatusType::SUCCESS);
        }
        res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        for (int i = 1; i <= 50; ++i)
        {
            StatusType res1 = obj->add_player(1000+i, 1, permutation_t::neutral(), 1, 1, 1, true);;
            REQUIRE(res1 == StatusType::SUCCESS);
        }
        output_t<int> res0 = obj->get_ith_pointless_ability(0);
        REQUIRE(res0.status() == StatusType::SUCCESS);
        REQUIRE(res0.ans() == 5);
        output_t<int> res1 = obj->get_ith_pointless_ability(1);
        REQUIRE(res1.status() == StatusType::SUCCESS);
        REQUIRE(res1.ans() == 4);
        output_t<int> res2 = obj->get_ith_pointless_ability(2);
        REQUIRE(res2.status() == StatusType::SUCCESS);
        REQUIRE(res2.ans() == 3);
        output_t<int> res3 = obj->get_ith_pointless_ability(3);
        REQUIRE(res3.status() == StatusType::SUCCESS);
        REQUIRE(res3.ans() == 2);
        output_t<int> res4 = obj->get_ith_pointless_ability(4);
        REQUIRE(res4.status() == StatusType::SUCCESS);
        REQUIRE(res4.ans() == 1);
        output_t<int> res5 = obj->get_ith_pointless_ability(5);
        REQUIRE(res5.status() == StatusType::FAILURE);
        res = obj->remove_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->remove_team(4);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res0n = obj->get_ith_pointless_ability(0);
        REQUIRE(res0n.status() == StatusType::SUCCESS);
        REQUIRE(res0n.ans() == 5);
        output_t<int> res1n = obj->get_ith_pointless_ability(1);
        REQUIRE(res1n.status() == StatusType::SUCCESS);
        REQUIRE(res1n.ans() == 3);
        output_t<int> res2n = obj->get_ith_pointless_ability(2);
        REQUIRE(res2n.status() == StatusType::SUCCESS);
        REQUIRE(res2n.ans() == 1);
        output_t<int> res3n = obj->get_ith_pointless_ability(3);
        REQUIRE(res3n.status() == StatusType::FAILURE);
        output_t<int> res4n = obj->get_ith_pointless_ability(4);
        REQUIRE(res4n.status() == StatusType::FAILURE);
        output_t<int> res5n = obj->get_ith_pointless_ability(5);
        REQUIRE(res5n.status() == StatusType::FAILURE);

        delete obj;
    }


    SECTION("some teams are removed then readded")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(5);
        REQUIRE(res == StatusType::SUCCESS);
        for (int i = 1; i <= 10; ++i)
        {
            StatusType res1 = obj->add_player(5000+i, 5, permutation_t::neutral(), 1, 1, 1, true);;
            REQUIRE(res1 == StatusType::SUCCESS);

        }
        res = obj->add_team(4);
        REQUIRE(res == StatusType::SUCCESS);
        for (int i = 1; i <= 20; ++i)
        {
            StatusType res1 = obj->add_player(4000+i, 4, permutation_t::neutral(), 1, 1, 1, true);;
            REQUIRE(res1 == StatusType::SUCCESS);
        }
        res = obj->add_team(3);
        REQUIRE(res == StatusType::SUCCESS);
        for (int i = 1; i <= 30; ++i)
        {
            StatusType res1 = obj->add_player(3000+i, 3, permutation_t::neutral(), 1, 1, 1, true);;
            REQUIRE(res1 == StatusType::SUCCESS);
        }
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        for (int i = 1; i <= 40; ++i)
        {
            StatusType res1 = obj->add_player(2000+i, 2, permutation_t::neutral(), 1, 1, 1, true);;
            REQUIRE(res1 == StatusType::SUCCESS);
        }
        res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        for (int i = 1; i <= 50; ++i)
        {
            StatusType res1 = obj->add_player(i, 1, permutation_t::neutral(), 1, 1, 1, true);;
            REQUIRE(res1 == StatusType::SUCCESS);
        }
        output_t<int> res0 = obj->get_ith_pointless_ability(0);
        REQUIRE(res0.status() == StatusType::SUCCESS);
        REQUIRE(res0.ans() == 5);
        output_t<int> res1 = obj->get_ith_pointless_ability(1);
        REQUIRE(res1.status() == StatusType::SUCCESS);
        REQUIRE(res1.ans() == 4);
        output_t<int> res2 = obj->get_ith_pointless_ability(2);
        REQUIRE(res2.status() == StatusType::SUCCESS);
        REQUIRE(res2.ans() == 3);
        output_t<int> res3 = obj->get_ith_pointless_ability(3);
        REQUIRE(res3.status() == StatusType::SUCCESS);
        REQUIRE(res3.ans() == 2);
        output_t<int> res4 = obj->get_ith_pointless_ability(4);
        REQUIRE(res4.status() == StatusType::SUCCESS);
        REQUIRE(res4.ans() == 1);
        output_t<int> res5 = obj->get_ith_pointless_ability(5);
        REQUIRE(res5.status() == StatusType::FAILURE);
        res = obj->remove_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->remove_team(4);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res0n = obj->get_ith_pointless_ability(0);
        REQUIRE(res0n.status() == StatusType::SUCCESS);
        REQUIRE(res0n.ans() == 5);
        output_t<int> res1n = obj->get_ith_pointless_ability(1);
        REQUIRE(res1n.status() == StatusType::SUCCESS);
        REQUIRE(res1n.ans() == 3);
        output_t<int> res2n = obj->get_ith_pointless_ability(2);
        REQUIRE(res2n.status() == StatusType::SUCCESS);
        REQUIRE(res2n.ans() == 1);
        output_t<int> res3n = obj->get_ith_pointless_ability(3);
        REQUIRE(res3n.status() == StatusType::FAILURE);
        output_t<int> res4n = obj->get_ith_pointless_ability(4);
        REQUIRE(res4n.status() == StatusType::FAILURE);
        output_t<int> res5n = obj->get_ith_pointless_ability(5);
        REQUIRE(res5n.status() == StatusType::FAILURE);


        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        for (int i = 1; i <= 49; ++i)
        {
            StatusType res1 = obj->add_player(7000+i, 2, permutation_t::neutral(), 1, 1, 1, true);;
            REQUIRE(res1 == StatusType::SUCCESS);
        }
        for (int i = 50; i <= 70; ++i)
        {
            StatusType res1 = obj->add_player(7000 + i, 2, permutation_t::neutral(), 1, 1, 1, true);;
            REQUIRE(res1 == StatusType::SUCCESS);
        }
        res = obj->add_team(4);
        REQUIRE(res == StatusType::SUCCESS);
        for (int i = 1; i <= 5; ++i)
        {
            StatusType res1 = obj->add_player(8000+i, 4, permutation_t::neutral(), 1, 1, 1, true);;
            REQUIRE(res1 == StatusType::SUCCESS);
        }

        output_t<int> res0a = obj->get_ith_pointless_ability(0);
        REQUIRE(res0a.status() == StatusType::SUCCESS);
        REQUIRE(res0a.ans() == 4);
        output_t<int> res1a = obj->get_ith_pointless_ability(1);
        REQUIRE(res1a.status() == StatusType::SUCCESS);
        REQUIRE(res1a.ans() == 5);
        output_t<int> res2a = obj->get_ith_pointless_ability(2);
        REQUIRE(res2a.status() == StatusType::SUCCESS);
        REQUIRE(res2a.ans() == 3);
        output_t<int> res3a = obj->get_ith_pointless_ability(3);
        REQUIRE(res3a.status() == StatusType::SUCCESS);
        REQUIRE(res3a.ans() == 1);
        output_t<int> res4a = obj->get_ith_pointless_ability(4);
        REQUIRE(res4a.status() == StatusType::SUCCESS);
        REQUIRE(res4a.ans() == 2);
        output_t<int> res5a = obj->get_ith_pointless_ability(5);
        REQUIRE(res5a.status() == StatusType::FAILURE);

        delete obj;
    }

    SECTION("there is a tie")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(5);
        REQUIRE(res == StatusType::SUCCESS);
        for (int i = 1; i <= 10; ++i)
        {
            StatusType res1 = obj->add_player(5000+i, 5, permutation_t::neutral(), 1, 1, 1, true);;
            REQUIRE(res1 == StatusType::SUCCESS);

        }
        res = obj->add_team(4);
        REQUIRE(res == StatusType::SUCCESS);
        for (int i = 1; i <= 10; ++i)
        {
            StatusType res1 = obj->add_player(4000+i, 4, permutation_t::neutral(), 1, 1, 1, true);;
            REQUIRE(res1 == StatusType::SUCCESS);
        }
        res = obj->add_team(3);
        REQUIRE(res == StatusType::SUCCESS);
        for (int i = 1; i <= 10; ++i)
        {
            StatusType res1 = obj->add_player(3000+i, 3, permutation_t::neutral(), 1, 1, 1, true);;
            REQUIRE(res1 == StatusType::SUCCESS);
        }
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        for (int i = 1; i <= 40; ++i)
        {
            StatusType res1 = obj->add_player(2000+i, 2, permutation_t::neutral(), 1, 1, 1, true);;
            REQUIRE(res1 == StatusType::SUCCESS);
        }
        res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        for (int i = 1; i <= 50; ++i)
        {
            StatusType res1 = obj->add_player(1000+i, 1, permutation_t::neutral(), 1, 1, 1, true);;
            REQUIRE(res1 == StatusType::SUCCESS);
        }
        output_t<int> res0 = obj->get_ith_pointless_ability(0);
        REQUIRE(res0.status() == StatusType::SUCCESS);
        REQUIRE(res0.ans() == 3);
        output_t<int> res1 = obj->get_ith_pointless_ability(1);
        REQUIRE(res1.status() == StatusType::SUCCESS);
        REQUIRE(res1.ans() == 4);
        output_t<int> res2 = obj->get_ith_pointless_ability(2);
        REQUIRE(res2.status() == StatusType::SUCCESS);
        REQUIRE(res2.ans() == 5);
        output_t<int> res3 = obj->get_ith_pointless_ability(3);
        REQUIRE(res3.status() == StatusType::SUCCESS);
        REQUIRE(res3.ans() == 2);
        output_t<int> res4 = obj->get_ith_pointless_ability(4);
        REQUIRE(res4.status() == StatusType::SUCCESS);
        REQUIRE(res4.ans() == 1);
        output_t<int> res5 = obj->get_ith_pointless_ability(5);
        REQUIRE(res5.status() == StatusType::FAILURE);

        delete obj;
    }
}


TEST_CASE("get_partial_spirit")
{
    SECTION("simple partial spirit")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1001, 1, permutation_t::read("4,5,3,1,2"), 1, 4, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1002, 1, permutation_t::read("2,4,3,1,5"), 0, 14, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<permutation_t> res1 = obj->get_partial_spirit(1001);
        REQUIRE(res1.status() == StatusType::SUCCESS);
        REQUIRE(res1.ans() == permutation_t::read("4,5,3,1,2"));
        output_t<permutation_t> res2 = obj->get_partial_spirit(1002);
        REQUIRE(res2.status() == StatusType::SUCCESS);
        REQUIRE(res2.ans() == permutation_t::read("5,1,3,4,2"));


        delete obj;
    }

    SECTION("not positive id")
    {
        world_cup_t* obj = new world_cup_t();

        output_t<permutation_t> res1 = obj->get_partial_spirit(-69);
        REQUIRE(res1.status() == StatusType::INVALID_INPUT);
        output_t<permutation_t> res2 = obj->get_partial_spirit(-5);
        REQUIRE(res2.status() == StatusType::INVALID_INPUT);
        output_t<permutation_t> res3 = obj->get_partial_spirit(0);
        REQUIRE(res3.status() == StatusType::INVALID_INPUT);
        output_t<permutation_t> res4 = obj->get_partial_spirit(-1);
        REQUIRE(res4.status() == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("player doesnt exist")
    {
        world_cup_t* obj = new world_cup_t();

        output_t<permutation_t> res1 = obj->get_partial_spirit(40);
        REQUIRE(res1.status() == StatusType::FAILURE);
        output_t<permutation_t> res2 = obj->get_partial_spirit(1);
        REQUIRE(res2.status() == StatusType::FAILURE);
        output_t<permutation_t> res3 = obj->get_partial_spirit(23404);
        REQUIRE(res3.status() == StatusType::FAILURE);
        output_t<permutation_t> res4 = obj->get_partial_spirit(3);
        REQUIRE(res4.status() == StatusType::FAILURE);

        delete obj;
    }

    SECTION("player is deposed")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1001, 1, permutation_t::read("4,5,3,1,2"), 1, 4, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1002, 1, permutation_t::read("3,4,2,1,5"), 0, 14, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2001, 2, permutation_t::read("5,1,3,4,2"), 1, 4, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2002, 2, permutation_t::read("2,4,3,1,5"), 0, 14, 0, true);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<permutation_t> res1 = obj->get_partial_spirit(1001);
        REQUIRE(res1.status() == StatusType::SUCCESS);
        output_t<permutation_t> res2 = obj->get_partial_spirit(2002);
        REQUIRE(res2.status() == StatusType::SUCCESS);

        res = obj->remove_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->remove_team(2);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<permutation_t> res3 = obj->get_partial_spirit(1002);
        REQUIRE(res3.status() == StatusType::FAILURE);
        output_t<permutation_t> res4 = obj->get_partial_spirit(2001);
        REQUIRE(res4.status() == StatusType::FAILURE);

        res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<permutation_t> res5 = obj->get_partial_spirit(1001);
        REQUIRE(res5.status() == StatusType::FAILURE);
        output_t<permutation_t> res6 = obj->get_partial_spirit(1002);
        REQUIRE(res6.status() == StatusType::FAILURE);


        delete obj;
    }


    SECTION("many partial spirits")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1001, 1, permutation_t::read("4,5,3,1,2"), 1, 4, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1002, 1, permutation_t::read("3,2,5,1,4"), 1, 4, 0, true);
        REQUIRE(res == StatusType::SUCCESS); //"3,5,2,4,1"
        res = obj->add_player(1003, 1, permutation_t::read("1,4,3,2,5"), 1, 4, 0, true);
        REQUIRE(res == StatusType::SUCCESS); //"3,4,2,5,1"
        res = obj->add_player(1004, 1, permutation_t::read("5,4,1,2,3"), 1, 4, 0, true);
        REQUIRE(res == StatusType::SUCCESS); //"1,5,3,4,2"
        res = obj->add_player(1005, 1, permutation_t::read("2,1,3,4,5"), 1, 4, 0, true);
        REQUIRE(res == StatusType::SUCCESS); //"5,1,3,4,2"
        res = obj->add_player(1006, 1, permutation_t::read("2,3,5,1,4"), 1, 4, 0, true);
        REQUIRE(res == StatusType::SUCCESS); //"1,3,2,5,4"
        res = obj->add_player(1007, 1, permutation_t::read("4,3,1,2,5"), 1, 4, 0, true);
        REQUIRE(res == StatusType::SUCCESS); //"5,2,1,3,4"
        res = obj->add_player(1008, 1, permutation_t::read("4,1,5,2,3"), 1, 4, 0, true);
        REQUIRE(res == StatusType::SUCCESS); //"3,5,4,2,1"
        res = obj->add_player(1009, 1, permutation_t::read("5,3,2,4,1"), 1, 4, 0, true);
        REQUIRE(res == StatusType::SUCCESS); //"1,4,5,2,3"
        res = obj->add_player(1010, 1, permutation_t::read("5,4,3,1,2"), 1, 4, 0, true);
        REQUIRE(res == StatusType::SUCCESS); //"3,2,5,1,4"
        res = obj->add_player(1011, 1, permutation_t::read("4,1,2,3,5"), 1, 4, 0, true);
        REQUIRE(res == StatusType::SUCCESS); //"1,3,2,5,4"
        res = obj->add_player(1012, 1, permutation_t::read("2,5,4,1,3"), 1, 4, 0, true);
        REQUIRE(res == StatusType::SUCCESS); //"3,4,5,1,2"
        res = obj->add_player(1013, 1, permutation_t::read("3,2,4,1,5"), 1, 4, 0, true);
        REQUIRE(res == StatusType::SUCCESS); //"5,4,1,3,2"

        output_t<permutation_t> res1 = obj->get_partial_spirit(1001);
        REQUIRE(res1.status() == StatusType::SUCCESS);
        REQUIRE(res1.ans() == permutation_t::read("4,5,3,1,2"));
        output_t<permutation_t> res2 = obj->get_partial_spirit(1002);
        REQUIRE(res2.status() == StatusType::SUCCESS);
        REQUIRE(res2.ans() == permutation_t::read("3,5,2,4,1"));
        output_t<permutation_t> res3 = obj->get_partial_spirit(1003);
        REQUIRE(res3.status() == StatusType::SUCCESS);
        REQUIRE(res3.ans() == permutation_t::read("3,4,2,5,1"));
        output_t<permutation_t> res4 = obj->get_partial_spirit(1004);
        REQUIRE(res4.status() == StatusType::SUCCESS);
        REQUIRE(res4.ans() == permutation_t::read("1,5,3,4,2"));
        output_t<permutation_t> res5 = obj->get_partial_spirit(1005);
        REQUIRE(res5.status() == StatusType::SUCCESS);
        REQUIRE(res5.ans() == permutation_t::read("5,1,3,4,2"));
        output_t<permutation_t> res6 = obj->get_partial_spirit(1006);
        REQUIRE(res6.status() == StatusType::SUCCESS);
        REQUIRE(res6.ans() == permutation_t::read("1,3,2,5,4"));
        output_t<permutation_t> res7 = obj->get_partial_spirit(1007);
        REQUIRE(res7.status() == StatusType::SUCCESS);
        REQUIRE(res7.ans() == permutation_t::read("5,2,1,3,4"));
        output_t<permutation_t> res8 = obj->get_partial_spirit(1008);
        REQUIRE(res8.status() == StatusType::SUCCESS);
        REQUIRE(res8.ans() == permutation_t::read("3,5,4,2,1"));
        output_t<permutation_t> res9 = obj->get_partial_spirit(1009);
        REQUIRE(res9.status() == StatusType::SUCCESS);
        REQUIRE(res9.ans() == permutation_t::read("1,4,5,2,3"));
        output_t<permutation_t> res10 = obj->get_partial_spirit(1010);
        REQUIRE(res10.status() == StatusType::SUCCESS);
        REQUIRE(res10.ans() == permutation_t::read("3,2,5,1,4"));
        output_t<permutation_t> res11 = obj->get_partial_spirit(1011);
        REQUIRE(res11.status() == StatusType::SUCCESS);
        REQUIRE(res11.ans() == permutation_t::read("1,3,2,5,4"));
        output_t<permutation_t> res12 = obj->get_partial_spirit(1012);
        REQUIRE(res12.status() == StatusType::SUCCESS);
        REQUIRE(res12.ans() == permutation_t::read("3,4,5,1,2"));
        output_t<permutation_t> res13 = obj->get_partial_spirit(1013);
        REQUIRE(res13.status() == StatusType::SUCCESS);
        REQUIRE(res13.ans() == permutation_t::read("5,4,1,3,2"));


        delete obj;
    }

}

TEST_CASE("buy_team")
{
    SECTION("simple buy team")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1001, 1, permutation_t::neutral(), 1, 4, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2001, 2, permutation_t::neutral(), 0, 14, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->buy_team(1, 2);
        REQUIRE(res == StatusType::SUCCESS);

        delete obj;
    }

    SECTION("empty teams")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(3);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(4);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(5);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(6);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->add_player(1001, 1, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2001, 2, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->buy_team(1, 3);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->buy_team(4, 2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->buy_team(5, 6);
        REQUIRE(res == StatusType::SUCCESS);


        delete obj;
    }

    SECTION("id are equal")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->buy_team(1, 1);
        REQUIRE(res == StatusType::INVALID_INPUT);
        res = obj->buy_team(2, 2);
        REQUIRE(res == StatusType::INVALID_INPUT);
        res = obj->buy_team(1, 2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->buy_team(1, 1);
        REQUIRE(res == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("id negative")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->buy_team(-1, 1);
        REQUIRE(res == StatusType::INVALID_INPUT);
        res = obj->buy_team(1, -1);
        REQUIRE(res == StatusType::INVALID_INPUT);
        res = obj->buy_team(2, 0);
        REQUIRE(res == StatusType::INVALID_INPUT);
        res = obj->buy_team(0, 2);
        REQUIRE(res == StatusType::INVALID_INPUT);
        res = obj->buy_team(2, 1);
        REQUIRE(res == StatusType::SUCCESS);

        delete obj;
    }

    SECTION("team does not exist")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->buy_team(3, 1);
        REQUIRE(res == StatusType::FAILURE);
        res = obj->buy_team(1, 4);
        REQUIRE(res == StatusType::FAILURE);
        res = obj->remove_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->buy_team(2, 1);
        REQUIRE(res == StatusType::FAILURE);
        res = obj->buy_team(1, 2);
        REQUIRE(res == StatusType::FAILURE);
        res = obj->remove_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->buy_team(2, 1);
        REQUIRE(res == StatusType::FAILURE);
        res = obj->buy_team(1, 2);
        REQUIRE(res == StatusType::FAILURE);

        delete obj;
    }

    SECTION("buy new removed team")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(3);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->remove_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->remove_team(3);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(3);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->buy_team(1, 2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->buy_team(3, 1);
        REQUIRE(res == StatusType::SUCCESS);

        delete obj;
    }

    SECTION("random teams buy")
    {
        world_cup_t* obj = new world_cup_t();
        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(3);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(4);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(5);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(6);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(7);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(8);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->add_player(1001, 1, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1002, 1, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1003, 1, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1004, 1, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1005, 1, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2001, 2, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2002, 2, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2003, 2, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3001, 3, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(4001, 4, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(4002, 4, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(4003, 4, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(5001, 5, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(6001, 6, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(7001, 7, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(7002, 7, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(8001, 8, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(8002, 8, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(8003, 8, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(8004, 8, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->buy_team(2, 3);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->buy_team(6, 1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->buy_team(7, 8);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->buy_team(5, 4);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->buy_team(2, 7);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->buy_team(5, 6);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->buy_team(2, 5);
        REQUIRE(res == StatusType::SUCCESS);

        delete obj;
    }

    SECTION("play match after buying GK")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(3);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->add_player(1001, 1, permutation_t::neutral(), 1, 1000, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1002, 1, permutation_t::neutral(), 1, 1, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2001, 2, permutation_t::neutral(), 1, 1, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2002, 2, permutation_t::neutral(), 1, 1, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3001, 3, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res1 = obj->play_match(1, 2);
        REQUIRE(res1.status() == StatusType::FAILURE);

        res = obj->buy_team(2, 3);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res2 = obj->play_match(1, 2);
        REQUIRE(res2.status() == StatusType::SUCCESS);
        REQUIRE(res2.ans() == 1);

        delete obj;
    }

    SECTION("team points after buy team")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(3);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(4);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(5);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->add_player(1001, 1, permutation_t::neutral(), 1, 1000, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1002, 1, permutation_t::neutral(), 1, 1, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2001, 2, permutation_t::neutral(), 1, 1000, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2002, 2, permutation_t::neutral(), 1, 1, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3001, 3, permutation_t::neutral(), 1, 1000, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(4001, 4, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(4002, 4, permutation_t::neutral(), 1, 1000, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(5001, 5, permutation_t::neutral(), 1, 0, 0, true);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res1 = obj->play_match(1, 5);
        REQUIRE(res1.status() == StatusType::SUCCESS);
        REQUIRE(res1.ans() == 1);
        output_t<int> res2 = obj->play_match(1, 5);
        REQUIRE(res2.status() == StatusType::SUCCESS);
        REQUIRE(res2.ans() == 1);
        output_t<int> res3 = obj->play_match(1, 5);
        REQUIRE(res3.status() == StatusType::SUCCESS);
        REQUIRE(res3.ans() == 1);
        output_t<int> res4 = obj->play_match(1, 5);
        REQUIRE(res4.status() == StatusType::SUCCESS);
        REQUIRE(res4.ans() == 1);
        output_t<int> res5 = obj->play_match(1, 5);
        REQUIRE(res5.status() == StatusType::SUCCESS);
        REQUIRE(res5.ans() == 1);
        output_t<int> res6 = obj->play_match(1, 5);
        REQUIRE(res6.status() == StatusType::SUCCESS);
        REQUIRE(res6.ans() == 1);
        output_t<int> res7 = obj->play_match(1, 5);
        REQUIRE(res7.status() == StatusType::SUCCESS);
        REQUIRE(res7.ans() == 1);
        output_t<int> res8 = obj->play_match(5, 2);
        REQUIRE(res8.status() == StatusType::SUCCESS);
        REQUIRE(res8.ans() == 3);
        output_t<int> res9 = obj->play_match(5, 2);
        REQUIRE(res9.status() == StatusType::SUCCESS);
        REQUIRE(res9.ans() == 3);
        output_t<int> res10 = obj->play_match(5, 2);
        REQUIRE(res10.status() == StatusType::SUCCESS);
        REQUIRE(res10.ans() == 3);
        output_t<int> res11 = obj->play_match(3, 5);
        REQUIRE(res11.status() == StatusType::SUCCESS);
        REQUIRE(res11.ans() == 1);
        output_t<int> res12 = obj->play_match(4, 5);
        REQUIRE(res12.status() == StatusType::SUCCESS);
        REQUIRE(res12.ans() == 1);
        output_t<int> res13 = obj->play_match(4, 5);
        REQUIRE(res13.status() == StatusType::SUCCESS);
        REQUIRE(res13.ans() == 1);

        output_t<int> res14 = obj->get_team_points(1);
        REQUIRE(res14.status() == StatusType::SUCCESS);
        REQUIRE(res14.ans() == 21);
        output_t<int> res15 = obj->get_team_points(2);
        REQUIRE(res15.status() == StatusType::SUCCESS);
        REQUIRE(res15.ans() == 9);
        output_t<int> res16 = obj->get_team_points(3);
        REQUIRE(res16.status() == StatusType::SUCCESS);
        REQUIRE(res16.ans() == 3);
        output_t<int> res17 = obj->get_team_points(4);
        REQUIRE(res17.status() == StatusType::SUCCESS);
        REQUIRE(res17.ans() == 6);
        output_t<int> res18 = obj->get_team_points(5);
        REQUIRE(res18.status() == StatusType::SUCCESS);
        REQUIRE(res18.ans() == 0);

        res = obj->buy_team(1, 2);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res19 = obj->get_team_points(1);
        REQUIRE(res19.status() == StatusType::SUCCESS);
        REQUIRE(res19.ans() == 30);

        res = obj->buy_team(3, 4);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res20 = obj->get_team_points(3);
        REQUIRE(res20.status() == StatusType::SUCCESS);
        REQUIRE(res20.ans() == 9);

        res = obj->buy_team(3, 5);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res21 = obj->get_team_points(3);
        REQUIRE(res21.status() == StatusType::SUCCESS);
        REQUIRE(res21.ans() == 9);

        res = obj->buy_team(3, 1);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res22 = obj->get_team_points(3);
        REQUIRE(res22.status() == StatusType::SUCCESS);
        REQUIRE(res22.ans() == 39);

        delete obj;
    }

    SECTION("player games after buy teams")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(3);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(4);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(5);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->add_player(1001, 1, permutation_t::neutral(), 0, 1000, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1002, 1, permutation_t::neutral(), 0, 1, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2001, 2, permutation_t::neutral(), 0, 1000, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2002, 2, permutation_t::neutral(), 0, 1, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3001, 3, permutation_t::neutral(), 0, 20000, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(4001, 4, permutation_t::neutral(), 0, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(4002, 4, permutation_t::neutral(), 0, 1000, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(5001, 5, permutation_t::neutral(), 0, 0, 0, true);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res1 = obj->play_match(1, 5);
        REQUIRE(res1.status() == StatusType::SUCCESS);
        REQUIRE(res1.ans() == 1);
        output_t<int> res2 = obj->play_match(1, 5);
        REQUIRE(res2.status() == StatusType::SUCCESS);
        REQUIRE(res2.ans() == 1);
        output_t<int> res3 = obj->play_match(1, 5);
        REQUIRE(res3.status() == StatusType::SUCCESS);
        REQUIRE(res3.ans() == 1);
        output_t<int> res4 = obj->play_match(1, 5);
        REQUIRE(res4.status() == StatusType::SUCCESS);
        REQUIRE(res4.ans() == 1);
        output_t<int> res5 = obj->play_match(1, 5);
        REQUIRE(res5.status() == StatusType::SUCCESS);
        REQUIRE(res5.ans() == 1);
        output_t<int> res6 = obj->play_match(1, 5);
        REQUIRE(res6.status() == StatusType::SUCCESS);
        REQUIRE(res6.ans() == 1);
        output_t<int> res7 = obj->play_match(1, 5);
        REQUIRE(res7.status() == StatusType::SUCCESS);
        REQUIRE(res7.ans() == 1);
        output_t<int> res8 = obj->play_match(5, 2);
        REQUIRE(res8.status() == StatusType::SUCCESS);
        REQUIRE(res8.ans() == 3);
        output_t<int> res9 = obj->play_match(5, 2);
        REQUIRE(res9.status() == StatusType::SUCCESS);
        REQUIRE(res9.ans() == 3);
        output_t<int> res10 = obj->play_match(5, 2);
        REQUIRE(res10.status() == StatusType::SUCCESS);
        REQUIRE(res10.ans() == 3);
        output_t<int> res11 = obj->play_match(3, 5);
        REQUIRE(res11.status() == StatusType::SUCCESS);
        REQUIRE(res11.ans() == 1);
        output_t<int> res12 = obj->play_match(4, 5);
        REQUIRE(res12.status() == StatusType::SUCCESS);
        REQUIRE(res12.ans() == 1);
        output_t<int> res13 = obj->play_match(4, 5);
        REQUIRE(res13.status() == StatusType::SUCCESS);
        REQUIRE(res13.ans() == 1);

        output_t<int> res14 = obj->num_played_games_for_player(1002);
        REQUIRE(res14.status() == StatusType::SUCCESS);
        REQUIRE(res14.ans() == 7);
        output_t<int> res15 = obj->num_played_games_for_player(2002);
        REQUIRE(res15.status() == StatusType::SUCCESS);
        REQUIRE(res15.ans() == 3);
        output_t<int> res16 = obj->num_played_games_for_player(3001);
        REQUIRE(res16.status() == StatusType::SUCCESS);
        REQUIRE(res16.ans() == 1);
        output_t<int> res17 = obj->num_played_games_for_player(4001);
        REQUIRE(res17.status() == StatusType::SUCCESS);
        REQUIRE(res17.ans() == 2);
        output_t<int> res18 = obj->num_played_games_for_player(5001);
        REQUIRE(res18.status() == StatusType::SUCCESS);
        REQUIRE(res18.ans() == 13);

        res = obj->buy_team(1, 2);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res19 = obj->num_played_games_for_player(1002);
        REQUIRE(res19.status() == StatusType::SUCCESS);
        REQUIRE(res19.ans() == 7);
        output_t<int> res20 = obj->num_played_games_for_player(2002);
        REQUIRE(res20.status() == StatusType::SUCCESS);
        REQUIRE(res20.ans() == 3);

        res = obj->buy_team(3, 4);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res21 = obj->num_played_games_for_player(3001);
        REQUIRE(res21.status() == StatusType::SUCCESS);
        REQUIRE(res21.ans() == 1);
        output_t<int> res22 = obj->num_played_games_for_player(4001);
        REQUIRE(res22.status() == StatusType::SUCCESS);
        REQUIRE(res22.ans() == 2);

        output_t<int> res23_1 = obj->play_match(3, 1);
        REQUIRE(res23_1.status() == StatusType::SUCCESS);
        REQUIRE(res23_1.ans() == 1);

        output_t<int> res23_2 = obj->play_match(3, 5);
        REQUIRE(res23_2.status() == StatusType::SUCCESS);
        REQUIRE(res23_2.ans() == 1);

        output_t<int> res23_3 = obj->num_played_games_for_player(1002);
        REQUIRE(res23_3.status() == StatusType::SUCCESS);
        REQUIRE(res23_3.ans() == 8);
        output_t<int> res23_4 = obj->num_played_games_for_player(2002);
        REQUIRE(res23_4.status() == StatusType::SUCCESS);
        REQUIRE(res23_4.ans() == 4);
        output_t<int> res23_5 = obj->num_played_games_for_player(3001);
        REQUIRE(res23_5.status() == StatusType::SUCCESS);
        REQUIRE(res23_5.ans() == 3);
        output_t<int> res23_6 = obj->num_played_games_for_player(4001);
        REQUIRE(res23_6.status() == StatusType::SUCCESS);
        REQUIRE(res23_6.ans() == 4);
        output_t<int> res23_7 = obj->num_played_games_for_player(5001);
        REQUIRE(res23_7.status() == StatusType::SUCCESS);
        REQUIRE(res23_7.ans() == 14);

        res = obj->buy_team(3, 5);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res23 = obj->num_played_games_for_player(3001);
        REQUIRE(res23.status() == StatusType::SUCCESS);
        REQUIRE(res23.ans() == 3);
        output_t<int> res24 = obj->num_played_games_for_player(4001);
        REQUIRE(res24.status() == StatusType::SUCCESS);
        REQUIRE(res24.ans() == 4);
        output_t<int> res25 = obj->num_played_games_for_player(5001);
        REQUIRE(res25.status() == StatusType::SUCCESS);
        REQUIRE(res25.ans() == 14);

        output_t<int> res25_2 = obj->play_match(3, 1);
        REQUIRE(res25_2.status() == StatusType::SUCCESS);
        REQUIRE(res25_2.ans() == 1);

        output_t<int> res25_3 = obj->num_played_games_for_player(1002);
        REQUIRE(res25_3.status() == StatusType::SUCCESS);
        REQUIRE(res25_3.ans() == 9);
        output_t<int> res25_4 = obj->num_played_games_for_player(2002);
        REQUIRE(res25_4.status() == StatusType::SUCCESS);
        REQUIRE(res25_4.ans() == 5);
        output_t<int> res25_5 = obj->num_played_games_for_player(3001);
        REQUIRE(res25_5.status() == StatusType::SUCCESS);
        REQUIRE(res25_5.ans() == 4);
        output_t<int> res25_6 = obj->num_played_games_for_player(4001);
        REQUIRE(res25_6.status() == StatusType::SUCCESS);
        REQUIRE(res25_6.ans() == 5);
        output_t<int> res25_7 = obj->num_played_games_for_player(5001);
        REQUIRE(res25_7.status() == StatusType::SUCCESS);
        REQUIRE(res25_7.ans() == 15);

        res = obj->buy_team(3, 1);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res26 = obj->num_played_games_for_player(1002);
        REQUIRE(res26.status() == StatusType::SUCCESS);
        REQUIRE(res26.ans() == 9);
        output_t<int> res27 = obj->num_played_games_for_player(2002);
        REQUIRE(res27.status() == StatusType::SUCCESS);
        REQUIRE(res27.ans() == 5);
        output_t<int> res28 = obj->num_played_games_for_player(3001);
        REQUIRE(res28.status() == StatusType::SUCCESS);
        REQUIRE(res28.ans() == 4);
        output_t<int> res29 = obj->num_played_games_for_player(4001);
        REQUIRE(res29.status() == StatusType::SUCCESS);
        REQUIRE(res29.ans() == 5);
        output_t<int> res30 = obj->num_played_games_for_player(5001);
        REQUIRE(res30.status() == StatusType::SUCCESS);
        REQUIRE(res30.ans() == 15);

        delete obj;
    }

    SECTION("partial spirit after buy")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(3);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(4);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(5);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->add_player(1001, 1, permutation_t::read("2,3,5,4,1"), 1, 1000, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1002, 1, permutation_t::neutral(), 1, 1, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2001, 2, permutation_t::read("4,2,1,5,3"), 1, 100, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2002, 2, permutation_t::neutral(), 1, 1, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3001, 3, permutation_t::read("5,4,3,2,1"), 1, 10000, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(4001, 4, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(4002, 4, permutation_t::read("1,5,2,4,3"), 1, 10, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(5001, 5, permutation_t::neutral(), 1, 0, 0, true);
        REQUIRE(res == StatusType::SUCCESS);


        output_t<permutation_t> res14 = obj->get_partial_spirit(1001);
        REQUIRE(res14.status() == StatusType::SUCCESS);
        REQUIRE(res14.ans() == permutation_t::read("2,3,5,4,1"));
        output_t<permutation_t> res15 = obj->get_partial_spirit(2001);
        REQUIRE(res15.status() == StatusType::SUCCESS);
        REQUIRE(res15.ans() == permutation_t::read("4,2,1,5,3"));
        output_t<permutation_t> res16 = obj->get_partial_spirit(3001);
        REQUIRE(res16.status() == StatusType::SUCCESS);
        REQUIRE(res16.ans() == permutation_t::read("5,4,3,2,1"));
        output_t<permutation_t> res17 = obj->get_partial_spirit(4002);
        REQUIRE(res17.status() == StatusType::SUCCESS);
        REQUIRE(res17.ans() == permutation_t::read("1,5,2,4,3"));
        output_t<permutation_t> res18 = obj->get_partial_spirit(5001);
        REQUIRE(res18.status() == StatusType::SUCCESS);
        REQUIRE(res18.ans() == permutation_t::neutral());

        res = obj->buy_team(1, 2);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<permutation_t> res140 = obj->get_partial_spirit(1001);
        REQUIRE(res140.status() == StatusType::SUCCESS);
        REQUIRE(res140.ans() == permutation_t::read("2,3,5,4,1"));
        output_t<permutation_t> res150 = obj->get_partial_spirit(2001);
        REQUIRE(res150.status() == StatusType::SUCCESS);
        REQUIRE(res150.ans() == permutation_t::read("4,3,2,1,5"));


        res = obj->buy_team(3, 4);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<permutation_t> res161 = obj->get_partial_spirit(3001);
        REQUIRE(res161.status() == StatusType::SUCCESS);
        REQUIRE(res161.ans() == permutation_t::read("5,4,3,2,1"));
        output_t<permutation_t> res171 = obj->get_partial_spirit(4002);
        REQUIRE(res171.status() == StatusType::SUCCESS);
        REQUIRE(res171.ans() == permutation_t::read("5,1,4,2,3"));



        res = obj->buy_team(3, 5);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<permutation_t> res162 = obj->get_partial_spirit(3001);
        REQUIRE(res162.status() == StatusType::SUCCESS);
        REQUIRE(res162.ans() == permutation_t::read("5,4,3,2,1"));
        output_t<permutation_t> res172 = obj->get_partial_spirit(4002);
        REQUIRE(res172.status() == StatusType::SUCCESS);
        REQUIRE(res172.ans() == permutation_t::read("5,1,4,2,3"));
        output_t<permutation_t> res182 = obj->get_partial_spirit(5001);
        REQUIRE(res182.status() == StatusType::SUCCESS);
        REQUIRE(res182.ans() == permutation_t::read("5,1,4,2,3"));

        res = obj->buy_team(3, 1);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<permutation_t> res143 = obj->get_partial_spirit(1001);
        REQUIRE(res143.status() == StatusType::SUCCESS);
        REQUIRE(res143.ans() == permutation_t::read("1,4,3,2,5"));
        output_t<permutation_t> res153 = obj->get_partial_spirit(2001);
        REQUIRE(res153.status() == StatusType::SUCCESS);
        REQUIRE(res153.ans() == permutation_t::read("2,4,1,5,3"));
        output_t<permutation_t> res163 = obj->get_partial_spirit(3001);
        REQUIRE(res163.status() == StatusType::SUCCESS);
        REQUIRE(res163.ans() == permutation_t::read("5,4,3,2,1"));
        output_t<permutation_t> res173 = obj->get_partial_spirit(4002);
        REQUIRE(res173.status() == StatusType::SUCCESS);
        REQUIRE(res173.ans() == permutation_t::read("5,1,4,2,3"));
        output_t<permutation_t> res183 = obj->get_partial_spirit(5001);
        REQUIRE(res183.status() == StatusType::SUCCESS);
        REQUIRE(res183.ans() == permutation_t::read("5,1,4,2,3"));


        delete obj;
    }

    SECTION("ith ability after buy teams")
    {
        world_cup_t* obj = new world_cup_t();

        StatusType res = obj->add_team(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(3);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(4);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(5);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->add_player(1001, 1, permutation_t::neutral(), 1, 1000, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1002, 1, permutation_t::neutral(), 1, 1, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2001, 2, permutation_t::neutral(), 1, 100, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2002, 2, permutation_t::neutral(), 1, 1, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3001, 3, permutation_t::neutral(), 1, 10000, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(4001, 4, permutation_t::neutral(), 1, 1, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(4002, 4, permutation_t::neutral(), 1, 10, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(5001, 5, permutation_t::neutral(), 1, 0, 0, true);
        REQUIRE(res == StatusType::SUCCESS);


        output_t<int> res14 = obj->get_ith_pointless_ability(0);
        REQUIRE(res14.status() == StatusType::SUCCESS);
        REQUIRE(res14.ans() == 5);
        output_t<int> res15 = obj->get_ith_pointless_ability(1);
        REQUIRE(res15.status() == StatusType::SUCCESS);
        REQUIRE(res15.ans() == 4);
        output_t<int> res16 = obj->get_ith_pointless_ability(2);
        REQUIRE(res16.status() == StatusType::SUCCESS);
        REQUIRE(res16.ans() == 2);
        output_t<int> res17 = obj->get_ith_pointless_ability(3);
        REQUIRE(res17.status() == StatusType::SUCCESS);
        REQUIRE(res17.ans() == 1);
        output_t<int> res18 = obj->get_ith_pointless_ability(4);
        REQUIRE(res18.status() == StatusType::SUCCESS);
        REQUIRE(res18.ans() == 3);

        res = obj->buy_team(1, 2);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res141 = obj->get_ith_pointless_ability(0);
        REQUIRE(res141.status() == StatusType::SUCCESS);
        REQUIRE(res141.ans() == 5);
        output_t<int> res151 = obj->get_ith_pointless_ability(1);
        REQUIRE(res151.status() == StatusType::SUCCESS);
        REQUIRE(res151.ans() == 4);
        output_t<int> res161 = obj->get_ith_pointless_ability(2);
        REQUIRE(res161.status() == StatusType::SUCCESS);
        REQUIRE(res161.ans() == 1);
        output_t<int> res171 = obj->get_ith_pointless_ability(3);
        REQUIRE(res171.status() == StatusType::SUCCESS);
        REQUIRE(res171.ans() == 3);


        res = obj->buy_team(3, 4);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res142 = obj->get_ith_pointless_ability(0);
        REQUIRE(res142.status() == StatusType::SUCCESS);
        REQUIRE(res142.ans() == 5);
        output_t<int> res152 = obj->get_ith_pointless_ability(1);
        REQUIRE(res152.status() == StatusType::SUCCESS);
        REQUIRE(res152.ans() == 1);
        output_t<int> res162 = obj->get_ith_pointless_ability(2);
        REQUIRE(res162.status() == StatusType::SUCCESS);
        REQUIRE(res162.ans() == 3);



        res = obj->buy_team(3, 5);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res143 = obj->get_ith_pointless_ability(0);
        REQUIRE(res143.status() == StatusType::SUCCESS);
        REQUIRE(res143.ans() == 1);
        output_t<int> res153 = obj->get_ith_pointless_ability(1);
        REQUIRE(res153.status() == StatusType::SUCCESS);
        REQUIRE(res153.ans() == 3);


        res = obj->buy_team(3, 1);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res1400 = obj->get_ith_pointless_ability(0);
        REQUIRE(res1400.status() == StatusType::SUCCESS);
        REQUIRE(res1400.ans() == 3);

        delete obj;
    }


}
