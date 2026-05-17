#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"

class Channel
{
    private:
        std::string name;
        std::string topic;
        std::map<int, Client*> _members;
    public :
        std::string getName() const;
        std::string getTopic() const;
        void setTopic(const std::string& topic);
        void setName(const std::string& name);

        // member management
        void addMember(Client* client);
        void removeMember(int fd);
        bool isMember(int fd) const;
        const std::map<int, Client*>& getMembers() const;
};


#endif