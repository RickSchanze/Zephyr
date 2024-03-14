/**
 * @file Event.h
 * @author Echo
 * @Date 2024/2/27
 * @brief 事件系统
 */

#ifndef ZEPHYR_EVENT_H
#define ZEPHYR_EVENT_H

#include <functional>
#include <string>
#include <type_traits>
#include <utility>

#include "spdlog/spdlog.h"

#include "Utils/Guid.h"

template <typename... Args>
class Delegate
{
public:
    /**
     * 构造Delegate
     * @param id 自己设定的ID
     * @param func 代理的function
     */
    Delegate(std::string id, std::function<void(Args...)> func)
        : m_function(std::move(func)), m_id(std::move(id)), m_valid(true)
    {
    }

    /**
     * 使用一个对象的方法构造Delegate
     * @tparam ObjectType
     * @tparam FuncType
     * @param id delegate id
     * @param object 对象指针
     * @param func 对象的方法
     */
    template <typename ObjectType, typename FuncType>
    Delegate(std::string id, ObjectType *object, FuncType func) : m_id(std::move(id))
    {
        m_function = [object, func](Args... args) { (object->*func)(args...); };
        m_valid = true;
    }

    /**
     * 构造一个Delegate,id由GUID生成
     * @param func
     */
    explicit Delegate(std::function<void(Args...)> func) : m_id(Guid().ToString())
    {
        m_function = std::move(func);
        m_valid = true;
    }

    /**
     * 使用一个对象的方法构造Delegate
     *
     * @note id由GUID生成d
     *
     * @tparam ObjectType 对象类型
     * @tparam FuncType 函数类型
     * @param object 对象指针
     * @param func 函数指针
     */
    template <typename ObjectType, typename FuncType>
        requires(!std::is_same_v<ObjectType, const char>)
    Delegate(ObjectType *object, FuncType func) : m_id(Guid().ToString())
    {
        m_function = [object, func](Args... args) { (object->*func)(args...); };
        m_valid = true;
    }

    /**
     * 移动构造 重新生成ID并将原来的设为无效
     * @param other
     */
    Delegate(Delegate &&other) noexcept
    {
        m_function = std::move(other.m_function);
        m_id = Guid().ToString();
        m_valid = other.m_valid;
        other.m_valid = false;
    }

    /**
     * 复制构造，会重新生成id
     * @param other
     */
    Delegate(const Delegate &other)
    {
        m_function = other.m_function;
        m_id = Guid().ToString();
        m_valid = other.m_valid;
    }

    template <class... InvokeArgs>
    void operator()(InvokeArgs &&...args) const
    {
        if (m_valid)
        {
            if constexpr (sizeof...(InvokeArgs) == 0)
            {
                m_function();
            }
            else
            {
                m_function(std::forward<InvokeArgs...>(args...));
            }
        }
        else
        {
            spdlog::error("called invalid delegate");
        }
    }

    auto operator<=>(const Delegate &rhs) const
    {
        return m_id <=> rhs.m_id;
    }

    /** 获取Delegate id */
    const std::string &GetID() const
    {
        return m_id;
    }
    /** 此Delegate是否有效 */
    bool IsValid() const
    {
        return m_valid;
    }

private:
    std::function<void(Args...)> m_function;
    std::string m_id;
    bool m_valid;
};

template <typename... Args>
class Event
{
public:
    Event() = default;
    // 禁止拷贝构造和赋值
    Event(const Event &) = delete;
    Event &operator=(const Event &) = delete;

    /** 使用其他的Delegate或者Lambda来初始化添加Event */
    template <typename DelegateType>
    void AddListener(DelegateType &&delegate)
    {
        m_event_listeners.emplace_back(std::forward<DelegateType>(delegate));
    }

    /** 添加Delegate */
    template <typename ObjectType, typename ClassFunc>
    void AddObjectListener(std::string id, ObjectType *obj, ClassFunc func)
    {
        m_event_listeners.emplace_back(id, obj, func);
    }

    /** 添加Delegate */
    template <typename ObjectType, typename ClassFunc>
        requires(!std::is_same_v<ObjectType, const char>)
    void AddObjectListener(ObjectType *obj, ClassFunc func)
    {
        m_event_listeners.emplace_back(obj, func);
    }

    /** 添加Delegate */
    template <typename Func>
    void AddListener(std::string id, Func func)
    {
        m_event_listeners.emplace_back(std::move(id), func);
    }

    /** 添加Delegate */
    void RemoveListener(std::string id)
    {
        auto it = std::find_if(m_event_listeners.begin(), m_event_listeners.end(),
                               [&id](const Delegate<Args...> &delegate) { return delegate.GetName() == id; });
        if (it != m_event_listeners.end())
        {
            m_event_listeners.erase(it);
        }
    }

    /** 清除所有Delegate */
    void ClearListener()
    {
        m_event_listeners.clear();
    }

    /**
     * 分发当前事件
     * 将会调用绑定到此事件上的所有函数
     * @tparam InvokeArgs
     * @param args
     */
    template <typename... InvokeArgs>
    void Broadcast(InvokeArgs &&...args)
    {
        for (auto &listener : m_event_listeners)
        {
            if (listener.IsValid())
            {
                if constexpr (sizeof...(InvokeArgs) == 0)
                {
                    listener();
                }
                else
                {
                    listener(std::forward<InvokeArgs...>(args...));
                }
            }
            else
            {
                spdlog::error("Invalid delegate: {}", listener.GetID());
            }
        }
    }

    [[nodiscard]] size_t Size() const { return m_event_listeners.size(); }

private:
    std::vector<Delegate<Args...>> m_event_listeners;
};

#endif//ZEPHYR_EVENT_H
