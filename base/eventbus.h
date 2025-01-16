#pragma once

#include <vector>
#include <mutex>
#include <unordered_map>

#include "singleton.h"
#include "datetime.h"
#include "variant.hpp"

namespace base
{

//
typedef mpark::variant<bool, int64_t, double, std::string, int, void*> Variant;
typedef std::vector<Variant> Variants;
typedef std::function<void(const std::string &, const Variants &)> Handler;

//
class EventBus : public Singleton<EventBus>
{
    std::mutex m_mtx;
    std::unordered_multimap<std::string, std::weak_ptr<Handler>>  m_listeners;
    uint64_t m_last_cleanup;

public:
    //
    std::shared_ptr<Handler> AddListener(std::string id, std::shared_ptr<Handler> function)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_listeners.insert(std::pair<std::string, std::weak_ptr<Handler>>(id, function));
        return function;
    }

    //
    std::shared_ptr<Handler> AddListener(std::string id, Handler function)
    {
        return AddListener(id, std::make_shared<Handler>(function));
    }

    //
    std::shared_ptr<Handler> AddListeners(std::vector<std::string> ids, std::shared_ptr<Handler> function)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        for (auto & id : ids)
        {
            m_listeners.insert(std::pair<std::string, std::weak_ptr<Handler>>(id, function));
        }
        return function;
    }

    //
    std::shared_ptr<Handler> AddListeners(std::vector<std::string> ids, Handler function)
    {
        return AddListeners(ids, std::make_shared<Handler>(function));
    }

    //
    int SendMessage(const std::string& id, const Variants& data = {})
    {
        std::vector<std::weak_ptr<Handler>> listeners;

        {
            std::lock_guard<std::mutex> lock(m_mtx);
            auto range = m_listeners.equal_range(id);

            for (auto it = range.first; it != range.second;)
            {
                if (it->second.expired())
                {
                    it = m_listeners.erase(it);
                }
                else
                {
                    // 存入临时对象
                    listeners.push_back((it++)->second);
                }
            }

            // 定时清除未触发的 listeners
            auto now = base::GetTimeMs();
            if (m_last_cleanup + 1000 * 60 < now)
            {
                m_last_cleanup = now;
                for (auto it = m_listeners.begin(); it != m_listeners.end();)
                {
                    if (it->second.expired())
                    {
                        it = m_listeners.erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
        }

        int count = 0;
        for (auto & listener : listeners)
        {
            if (!listener.expired())
            {
                (*listener.lock())(id, data);
                count++;
            }
        }
        return count;
    }
};

}; // namespace base