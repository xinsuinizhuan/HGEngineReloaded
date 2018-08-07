#pragma once

#include <Behaviour.hpp>
#include <LogsListener.hpp>

namespace STD_MODULE_NS::Behaviours
{
    /**
     * @brief Standard behaviour, that implements
     * ingame console.
     */
    class IngameConsole : public CORE_MODULE_NS::Behaviour
    {
    public:

        struct Command
        {
            using Arguments = std::vector<std::string>;

            using Function = std::function<int(Arguments)>;

            Command() :
                command(),
                description(),
                function()
            {}

            Command(std::string command,
                    std::string desc,
                    Function func) :
                command(std::move(command)),
                description(std::move(desc)),
                function(std::move(func))
            {}

            std::string command;

            std::string description;

            Function function;
        };

        /**
         * @brief Constructor.
         */
        IngameConsole();

        /**
         * @brief Destructor.
         */
        ~IngameConsole() override;

        /**
         * @brief Method for adding command to ingame console.
         * @param c Command.
         */
        void addCommand(Command c);

        /**
         * @brief Method for toggling show/hide.
         */
        void toggle();

    protected:

        void onUpdate() override;

    private:

        class LoggingWatcher : public Logger::LogsListener
        {
        public:
            explicit LoggingWatcher(IngameConsole* ingameConsole);

            AbstractLogger::Message popMessage() override;

            bool hasMessages() const override;

            void newMessage(const AbstractLogger::Message& m) override;

        private:
            IngameConsole* m_console;

            std::queue<AbstractLogger::Message> m_messages;
        };

        void displayConsole();

        void connectLoggingWatcher();

        void proceedLogs();

        void proceedMessage(AbstractLogger::Message message);

        void logText(const UTILS_MODULE_NS::Color& color, std::string text);

        std::string formatMessage(AbstractLogger::Message message);

        UTILS_MODULE_NS::Color getLogColor(AbstractLogger::ErrorClass errClass);

        void executeCommand(std::string command);

        template<typename T>
        bool toggleBehaviour(const Command::Arguments& arguments)
        {

            if (arguments.size() < 1 ||
                (arguments[0] != "0" &&
                 arguments[0] != "1"))
            {
                return false;
            }

            auto behaviour = gameObject()->findBehaviour<T>();

            if (behaviour && arguments[0] == "0")
            {
                if (behaviour->isEnabled())
                {
                    behaviour->setEnabled(false);
                }
                else
                {
                    logText(
                        getLogColor(AbstractLogger::ErrorClass::Error),
                        "Already disabled"
                    );
                }
            }
            else if (behaviour && arguments[0] == "1")
            {
                if (behaviour->isEnabled())
                {
                    logText(
                        getLogColor(AbstractLogger::ErrorClass::Error),
                        "Already enabled"
                    );
                }
                else
                {
                    behaviour->setEnabled(true);
                }
            }
            else if (!behaviour && arguments[0] == "0")
            {
                logText(
                    getLogColor(AbstractLogger::ErrorClass::Error),
                    "Already disabled"
                );
            }
            else if (!behaviour && arguments[0] == "1")
            {
                gameObject()->addBehaviour(new T);
            }

            return true;
        }

        // Commands
        int commandClEnableDebug(Command::Arguments arguments);

        Logger::LogsListenerPtr m_logsListener;

        std::map<std::string, Command> m_commands;

        // todo: Replace with ringbuffer
        std::vector<
            std::pair<
                UTILS_MODULE_NS::Color,
                std::string
            >
        > m_linesBuffer;

        bool m_shown;
    };
}

