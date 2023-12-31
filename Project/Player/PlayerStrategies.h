#pragma once
#include <vector>
#include <deque>
#include <string>
#include "../Map/Map.h"
#include "../Cards/Cards.h"

class Orders;
class OrdersList;
class Player;





// Abstract class
class PlayerStrategy{

    public:

        PlayerStrategy(); // Default constructor
        virtual ~PlayerStrategy();
        PlayerStrategy(const PlayerStrategy& strategy); // Copy constructor
        

        virtual bool issueOrder(Player *player, Deck* deck, Map* territoriesMap, const std::vector<Player*> gamePlayers) = 0;
        virtual std::vector<Territory*> toAttack(Player *player) = 0; // Priority of territories to attack
        virtual std::vector<Territory*> toDefend(Player *player) = 0; // which of their own territories are to be defended in priority


        // Helpers
        void setStrategyType(string name);
        void setCallingPlayer(Player*);
        string getStrategyType() const;
        Player* getCallingPlayer() const;


        // Stream insertion operator
        friend std::ostream& operator<<(std::ostream& out, const PlayerStrategy& strategy);

        // Assignment Operator
        PlayerStrategy& operator =(const PlayerStrategy& strategy);



    private:
        string strategyName;
        Player* callingPlayer;
};



// User interaction player
class HumanPlayerStrategy : public PlayerStrategy {

    public:
        HumanPlayerStrategy();
        HumanPlayerStrategy(Player* p);
        ~HumanPlayerStrategy();
        HumanPlayerStrategy(const HumanPlayerStrategy&);



        std::vector<Territory*> toAttack(Player *player) override;
        std::vector<Territory*> toDefend(Player *player) override;
        bool issueOrder(Player *player, Deck* deck, Map* territoriesMap, const std::vector<Player*> gamePlayers) override;

        // Stream insertion operator
        friend std::ostream& operator<<(std::ostream& out, const HumanPlayerStrategy& strategy);

         // Assignment Operator
        HumanPlayerStrategy& operator=(const HumanPlayerStrategy&);
};



/**
 * Aggressive player: computer player that focuses on attack (deploys or advances armies on its strongest 
 * country (most units), then always advances to enemy territories until it cannot do so anymore; will use any card with an 
 * aggressive purpose, as defined above)
 * 
*/
class AggressivePlayerStrategy : public PlayerStrategy {


    public:
        AggressivePlayerStrategy();
        ~AggressivePlayerStrategy();
        AggressivePlayerStrategy(const AggressivePlayerStrategy&);


        bool issueOrder(Player *player, Deck* deck, Map* territoriesMap, const std::vector<Player*> gamePlayers) override;
        std::vector<Territory*> toAttack(Player *player) override;
        std::vector<Territory*> toDefend(Player *player) override;

        Territory* get_strongest_territory();
        void set_strongest_territory(Player* player);


        // Stream insertion operator
        friend std::ostream& operator<<(std::ostream& out, const AggressivePlayerStrategy& strategy);

         // Assignment Operator
        AggressivePlayerStrategy& operator=(const AggressivePlayerStrategy&);

        
    private:
        Territory* strongest_territory;
        bool canAdvance;

};


/**
 * 
 * Benevolent player: computer player that focuses on protecting its weak countries (deploys or advances armies 
 * on its weakest countries (least units)), never advances to enemy territories; may use cards but will never use a card in a way 
 * that purposefully will harm anyone)
 * 
*/
class BenevolentPlayerStrategy : public PlayerStrategy {

    public:
        BenevolentPlayerStrategy();
        ~BenevolentPlayerStrategy();
        BenevolentPlayerStrategy(const BenevolentPlayerStrategy&);

        std::vector<Territory*> toAttack(Player *player) override;
        std::vector<Territory*> toDefend(Player *player) override;
        bool issueOrder(Player *player, Deck* deck, Map* territoriesMap, const std::vector<Player*> gamePlayers) override;


        //Helpers 
        Territory* get_weakest_territory();
        Territory* set_weakest_territory(Player* player);
        std::vector<std::pair<Territory*, int>> set_List_of_weakest_territories(Player* player);
        void negotiate(Player *player,Deck *deck);
        void airlift(Player *player,Deck *deck);


        // Stream insertion operator
        friend std::ostream& operator<<(std::ostream& out, const BenevolentPlayerStrategy& strategy);

         // Assignment Operator
        BenevolentPlayerStrategy& operator=(const BenevolentPlayerStrategy&);

    private:
        Territory* weakest_territory;
        std::vector<std::pair<Territory*, int>> stress_list;
        bool canAdvance;

};




/**
 * This strategy will make sure the player never issues any order.
 */
class NeutralPlayerStrategy : public PlayerStrategy {

    public:
        NeutralPlayerStrategy();
        ~NeutralPlayerStrategy();
        NeutralPlayerStrategy(const NeutralPlayerStrategy& strategy);


        std::vector<Territory*> toAttack(Player *player) override;
        std::vector<Territory*> toDefend(Player *player) override;
        bool issueOrder(Player *player, Deck* deck, Map* territoriesMap, const std::vector<Player*> gamePlayers) override;

        // Stream insertion operator
        friend std::ostream& operator<<(std::ostream& out, const NeutralPlayerStrategy& strategy);

        // Assignment Operator
        NeutralPlayerStrategy& operator =(const NeutralPlayerStrategy& strategy);

    private:
        int previousCardCount;
};



/**
 * Cheater player: computer player that automatically conquers all territories that are adjacent to its own 
 * territories (only once per turn). Does not use cards, though it may have or receive cards.
*/
class CheaterPlayerStrategy : public PlayerStrategy {
    public:
        CheaterPlayerStrategy();
        ~CheaterPlayerStrategy();
        CheaterPlayerStrategy(const CheaterPlayerStrategy&);

        bool issueOrder(Player *player, Deck* deck, Map* territoriesMap, const std::vector<Player*> gamePlayers) override;
        std::vector<Territory*> toAttack(Player *player) override;
        std::vector<Territory*> toDefend(Player *player) override;

        // Stream insertion operator
        friend std::ostream& operator<<(std::ostream& out, const CheaterPlayerStrategy& strategy);

        // Assignment Operator
        CheaterPlayerStrategy& operator =(const CheaterPlayerStrategy& strategy);
    
};
