#include "../includes/Channel.hpp"


    Channel::Channel() : name(""), topic("") , _inviteOnly(false), _topicRestricted(false), _key(""), _userLimit(0)
    {
        _members.clear();
    }

    Channel::Channel(const std::string& name) : name(name), topic("")
    {
        _members.clear();
    }

    Channel::~Channel()
    {
        _members.clear();
    }

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


    bool Channel::isOperator(Client *client)
    {
        return (_operators.find(client->getFD()) != _operators.end());
    }

    void Channel::addOperator(Client *client)
    {
        this->_operators.insert(client->getFD());
    }

    void Channel::removeOperator(Client *client)
    {
        this->_operators.erase(client->getFD());
    }

    const std::set<int>& Channel::getOperators() const
    {
        return _operators;
    }




void Channel::inviteClient(Client *client)
{
    _invited.insert(client->getFD());
}




bool Channel::isInvited(int fd)
{
    return (_invited.find(fd) != _invited.end());
}

void Channel::removeInvite(int fd)
{
    _invited.erase(fd);
}


bool Channel::isInviteOnly() const
{
    return _inviteOnly;
}





//for mode commands
void Channel::setInviteOnly(bool inviteOnly)
{
    _inviteOnly = inviteOnly;
}

bool Channel::isTopicRestricted() const
{
    return _topicRestricted;
}

void Channel::setTopicRestricted(bool topicRestricted)
{
    _topicRestricted = topicRestricted;
}

bool Channel::hasKey() const
{
    return !_key.empty();
}

const std::string &Channel::getKey() const
{
    return _key;
}

void Channel::setKey(const std::string &key)
{
    _key = key;
}

void Channel::removeKey()
{
    _key.clear();
}

bool Channel::hasLimit() const
{
    return _userLimit > 0;
}

size_t Channel::getUserLimit() const
{
    return _userLimit;
}
void Channel::setUserLimit(size_t limit)
{
    _userLimit = limit;
}
void Channel::removeUserLimit()
{
    _userLimit = 0;
}
