//***************************************************************************************
// helper.h by potoshopDev (C) 2023 All Rights Reserved.
//***************************************************************************************
#pragma once

#include <memory>

namespace dx
{
    template <class T>
    class getset final
    {
    private:
        class igetsetobject
        {
        public:
            virtual ~igetsetobject() {}
            inline virtual T get() const noexcept = 0;
            inline virtual void set(T t) noexcept = 0;
        };

        class getsetobject final : public igetsetobject
        {
        private:
            T m_data;

        public:
            getsetobject(T t) : m_data(std::move(t)) {}
            inline void set(T t) noexcept override { m_data = std::move(t); }
            inline T get() const noexcept override { return m_data; }
        };

    private:
        std::unique_ptr<igetsetobject> m_self;

    public:
        getset(T t) : m_self(std::make_unique<getsetobject>(std::move(t))) {}
        getset(getset &&g) = default;

        inline getset &operator=(const T &t) noexcept
        {
            T tmp{t};
            m_sefl = std::move(tmp);
            return *this;
        }

        inline getset &operator=(getset &g) noexcept
        {
            T tmp{g.get()};
            m_sefl = std::move(tmp);
            return *this;
        }

        T get() const noexcept { return m_self->get(); }
        void set(T t) { m_self->set(t); }
    };
} // namespace dx