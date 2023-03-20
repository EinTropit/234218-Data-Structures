#include "Hash.h"

Hash::Hash() : size(0), arrSize(STARTING_SIZE), players(new Node<Player>*[STARTING_SIZE])
{
    for (int i = 0; i < arrSize; ++i)
    {
        players[i] = nullptr;
    }
}

Hash::~Hash()
{
    Node<Player>* cur, *toDelete;
    for (int i = 0; i < arrSize; ++i)
    {
        if (players[i])
        {
            cur = players[i];
            while (cur != nullptr)
            {
                delete cur->value;
                toDelete = cur;
                cur = cur->next;
                delete toDelete;
            }
        }
    }
    delete[] players;
}

int Hash::h(int playerID) const
{
    return (playerID % arrSize);
}

Player *Hash::find(int playerID)
{
    Node<Player>* temp = players[h(playerID)];

    while (temp != nullptr)
    {
        if (temp->value->getId() == playerID)
            return temp->value;
        temp = temp->next;
    }
    return nullptr;
}

void Hash::insert(Player *player)
{
    if (find(player->getId()) != nullptr)
        throw KeyExists();

    Node<Player>* playerNode =  new Node<Player>(player);
    int id = player->getId();

    playerNode->next = players[h(id)];
    players[h(id)] = playerNode;
    size++;

    if (size >= arrSize)
        increaseSize();
}

void Hash::increaseSize()
{
    arrSize *= 2;
    Node<Player>** newArr = new Node<Player>*[arrSize];
    for (int i = 0; i < arrSize; ++i)
    {
        newArr[i] = nullptr;
    }

    Node<Player> *currNode, *temp;
    Player* player;
    for (int i = 0; i < arrSize/2; ++i)
    {
        currNode = players[i];

        while (currNode != nullptr)
        {
            temp = currNode->next;

            player = currNode->value;
            currNode->next = newArr[h(player->getId())];
            newArr[h(player->getId())] = currNode;

            currNode = temp;
        }
    }

    Node<Player>** toDelete = players;
    players = newArr;
    delete[] toDelete;
}