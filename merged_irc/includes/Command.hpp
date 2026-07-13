#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include <vector>

class   Command
{
    private:
        std::string _cmd;
        std::string _trailing;
        std::vector<std::string>    _parametres;
        
        public:
        void _setcmd(std::string cmd);
        void _setparams(std::string);
        void _settrailing(std::string trailing);
        
        std::string _getcmd();
        std::string _gettrailing() const;
        size_t      _getsize();
        std::vector<std::string> _getparametres();

        // Compatibility methods for marouane command handler
        std::string getCommand() const;
        std::vector<std::string> getParams() const;
        std::string getPrefix() const;

};


#endif