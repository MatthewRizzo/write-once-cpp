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

    explicit operator bool() const
    {
        return IsInitialized();
    }

    /// @return The value if it has been init, else a nullptr
    const T& Get() const
    {
        assert( IsInitialized() );
        return *m_pVal.get();
    }

    bool operator!() const
    {
        return IsInitialized();
    }

    const T& operator *() const
    {
        assert( IsInitialized() );
        return Get();
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

template<class T>
inline bool operator == ( WriteOnce<T> const& x, WriteOnce<T> const& y )
{
    return ( !x ) != ( !y ) ? false : ( !x ? true : ( *x == *y ) );
}

template<class T>
inline bool operator != ( WriteOnce<T> const& x, WriteOnce<T> const& y )
{
    return !( x == y );
}

template<class T>
inline bool operator < ( WriteOnce<T> const& x, WriteOnce<T> const& y )
{
    return !y ? false : ( !x ? true : ( *x ) < ( *y ) );
}

template<class T>
inline bool operator > ( WriteOnce<T> const& x, WriteOnce<T> const& y )
{
    return y < x;
}

template<class T>
inline bool operator <= ( WriteOnce<T> const& x, WriteOnce<T> const& y )
{
    return !( y < x );
}

template<class T>
inline bool operator >= ( WriteOnce<T> const& x, WriteOnce<T> const& y )
{
    return !( x < y );
}

template<class T>
inline constexpr bool operator==( const WriteOnce<T>& opt, const T& value )
{
    return bool(opt) ? *opt == value : false;
}

template<class T>
inline constexpr bool operator==( const T& value, const WriteOnce<T>& opt )
{
    return bool(opt) ? value == *opt : false;
}

template<class T>
inline constexpr bool operator!=( const WriteOnce<T>& opt, const T& value )
{
    return bool(opt) ? *opt != value : true;
}

template<class T>
inline constexpr bool operator!=( const T& value, const WriteOnce<T>& opt )
{
    return bool(opt) ? value != *opt : true;
}

template<class T>
inline constexpr bool operator<( const WriteOnce<T>& opt, const T& value )
{
    return bool(opt) ? *opt < value  : true;
}

template<class T>
inline constexpr bool operator<( const T& value, const WriteOnce<T>& opt )
{
    return bool(opt) ? value < *opt  : false;
}

template<class T>
inline constexpr bool operator<=( const WriteOnce<T>& opt, const T& value )
{
    return bool(opt) ? *opt <= value : true;
}

template<class T>
inline constexpr bool operator<=( const T& value, const WriteOnce<T>& opt )
{
    return bool(opt) ? value <= *opt : false;
}

template< class T>
inline constexpr bool operator>( const WriteOnce<T>& opt, const T& value )
{
    return bool(opt) ? *opt > value  : false;
}

template<class T>
inline constexpr bool operator>( const T& value, const WriteOnce<T>& opt )
{
    return bool(opt) ? value > *opt  : true;
}

template<class T>
inline constexpr bool operator>=( const WriteOnce<T>& opt, const T& value )
{
    return bool(opt) ? *opt >= value : false;
}

template<class T>
inline constexpr bool operator>=( const T& value, const WriteOnce<T>& opt )
{
    return bool(opt) ? value >= *opt : true;
}
