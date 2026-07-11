#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"

class Channel
{
    private:
        std::string name;
        std::string topic;
        std::map<int, Client*> _members;
        std::set<int> _operators;
        
        //for mode +i and invite commands
        bool _inviteOnly;
        bool _topicRestricted;
        std::string _key;
        std::set<int> _invited;
        size_t _userLimit;
    public :
        Channel();
        Channel(const std::string& name);
        ~Channel();
        std::string getName() const;
        std::string getTopic() const;
        void setTopic(const std::string& topic);
        void setName(const std::string& name);

        // member management
        void addMember(Client* client);
        void removeMember(int fd);
        bool isMember(int fd) const;
        const std::map<int, Client*>& getMembers() const;

        //operators operations
        bool isOperator(Client *client);
        void addOperator(Client *client);
        void removeOperator(Client *client);
        const std::set<int>& getOperators() const;

        //for client invite
        void inviteClient(Client *client);
        bool isInvited(int fd);
        void removeInvite(int fd);
        bool isInviteOnly() const;

        //for mode commands
        void setInviteOnly(bool);
        bool isTopicRestricted() const;
        void setTopicRestricted(bool);
        bool hasKey() const;
        const std::string &getKey() const;
        void setKey(const std::string &);
        void removeKey();
        bool hasLimit() const;
        size_t getUserLimit() const;
        void setUserLimit(size_t);
        void removeUserLimit();



};


#endif