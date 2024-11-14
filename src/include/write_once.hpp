////////////////////////////////////////////////////////////////////////////////
/// @author Matt Rizzo
/// @date Nov 13, 2024
/// @brief Template library allowing for a the assignment to a variable once and
///        only once.
////////////////////////////////////////////////////////////////////////////////

template<class T>
class WriteOnce
{
public:
    WriteOnce() :
        m_pVal(nullptr)
    {}
    WriteOnce(T&& val) :
        m_pVal( std::unique_ptr<T>( new T(std::move(val))) )
    {

    }

    ~WriteOnce() = default;

    /// @return True if the value was written
    bool Write(const T& val)
    {
        if(m_pVal != nullptr)
        {
            return false;
        }

        Assign(std::forward<const T&>(val));
        return true;
    }

    /// @return The value if it has been init, else a nullptr
    const T& Get() const
    {
        // assert( IsInitialized() );
        assert( IsInitialized() );
        return *m_pVal.get();
    }

    bool operator!() const
    {
        return IsInitialized();
    }

    T const* operator->() const
    {
        return m_pVal;
    }

// Private functions
private:
    void Assign(const T& val)
    {
        m_pVal = std::unique_ptr<T>(new T(val));
    }
    void Assign(T&& val)
    {
        m_pVal.reset(std::unique_ptr<T>(new T(std::move(val))));
    }

    bool IsInitialized() const
    {
        return m_pVal != nullptr;
    }

// Private data
private:
    // Allows us to delay initialization!
    std::unique_ptr<T> m_pVal;

};
