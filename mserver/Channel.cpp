#include "Channel.hpp"


    std::string Channel::getName() const
    {
        return name;
    }

    std::string Channel::getTopic() const
    {
        return topic;
    }

    void Channel::setName(const std::string& name)
    {
        this->name = name;
    }

    void Channel::setTopic(const std::string& topic)
    {
        this->topic = topic;
    }

    // member management
    void Channel::addMember(Client* client)
    {
        _members[client->getFD()] = client;
    }

    void Channel::removeMember(int fd)
    {
        _members.erase(fd);
    }

    bool Channel::isMember(int fd) const
    {
        return (_members.find(fd) != _members.end());
    }

    const std::map<int, Client*>& Channel::getMembers() const
    {
        return _members;
    }
