#pragma once
#include <cstring>

template<typename key_T = const char *, typename value_T = long>
struct Pair
{
    key_T key;
    value_T value;
    Pair(key_T key = key_T(), value_T value = value_T()): key(key), value(value){}
};


template<typename T>
bool is_less(T t1, T t2)
{
    return t1 < t2;
}

template<typename T>
bool is_equal(T t1, T t2)
{
    return t1 == t2;
}

template<>
inline bool is_less(const char* t1, const char* t2)
{
    return strcmp(t1,t2)<0;
}

template<>
inline bool is_equal(const char* t1, const char* t2)
{
    return strcmp(t1,t2)==0;
}


template<typename key_T, typename value_T>
class Dictionary
{
    static const int size_step=10;
    unsigned int m_size{0};
    unsigned int end_index{0};
    Pair<key_T,value_T>* data{nullptr};
    void clean()
    {
            if(data!=nullptr)
            {
                delete[] data;
            }
            m_size=0;
            end_index=0;
            data=nullptr;
    }
    int lower_bound(const key_T& key) const
    {
        int first =0;
        if(end_index>first)
        {
            int last = end_index;
            int count = last-first;
            int step,it;
            while (count > 0)
            {
                it = first; 
                step = count / 2; 
                it+=step;
        
                if (it<end_index && is_less(data[it].key , key))
                {
                    first = ++it; 
                    count -= step + 1; 
                }
                else
                    count = step;
            }
        }
        return first;
    }
    int binary_search(const key_T& key) const
    {
        int first = lower_bound(key);
        return (first != end_index && is_equal(key, data[first].key)) ? first : end_index;
    }
    int add_or_set(const Pair<key_T,value_T>& p)
    {
        int i_insert = lower_bound(p.key);
        bool should_resize;
        if(end_index>=m_size)
        {
            m_size+=size_step;
            should_resize;
        }
        end_index++;
        if(!should_resize)
        {
            for(int i=end_index-1;i>i_insert;i--)
            {
                data[i]=data[i-1];
            }
            data[i_insert] = Pair<key_T,value_T>(p.key, p.value);
        }
        else
        {
            Pair<key_T,value_T>* new_data = new Pair<key_T,value_T>[m_size];
            int retrieve_i;
            for(int i=0;i<end_index;i++)
            {
                retrieve_i = i<i_insert ? i:i-1;
                new_data[i] = i==i_insert?Pair<key_T,value_T>(p.key, p.value) : Pair<key_T,value_T>(data[retrieve_i].key, data[retrieve_i].value);
            }
            delete[] data;
            data = new_data;
        }
        return i_insert;
    }
    public:
        Dictionary(): Dictionary(Dictionary::size_step){}
        Dictionary(unsigned int size):m_size(size), end_index(0), data(new Pair<key_T,value_T>[size]){}
        Dictionary(unsigned int size, const Pair<key_T,value_T>& val): Dictionary(size)
        {
            end_index = size;
            for(int i=0;i<size;i++)
            {
                data[i]=Pair<key_T,value_T>(val.key, val.value);
            }
        }
        Dictionary(const Dictionary& other): Dictionary(other.m_size)
        {
            end_index = other.end_index;
            for(int i=0;i<m_size;i++)
            {
                data[i]=Pair<key_T,value_T>(other.data[i].key, other.data[i].value);
            }
        }
        Dictionary(Dictionary&& other)
        {
            m_size = other.size;
            end_index = other.end_index;
            data = other.data;

            other.m_size=0;
            other.end_index=0;
            other.data = nullptr;
        }
       ~Dictionary()
        {
            clean();
        }
        Dictionary<key_T,value_T>& operator=(const Dictionary& other)
        {
            if(this != &other)
            {
                this->clean();
                this->m_size=other.m_size;
                this->data= new Pair<key_T,value_T>[m_size];
                this->end_index = other.end_index;
                for(int i=0;i<m_size;i++)
                {
                    data[i]=Pair<key_T,value_T>(other.data[i].key, other.data[i].value);
                }
            }
            return *this;
        }

        Dictionary<key_T,value_T>& operator=(Dictionary&& other)
        {
            if(this != &other)
            {
                m_size = other.size;
                end_index = other.end_index;
                data = other.data;

                other.m_size=0;
                other.end_index=0;
                other.data = nullptr;
            }
            return *this;
        }
        int size() const { return end_index;}
        int find(const key_T& key) const {return binary_search(key);}
        value_T at_or_default(const key_T& key, value_T default_value = value_T()) const
        {
            int index = binary_search(key);
            if(index!=end_index)
            {
                return data[index].value;
            }
            return default_value;         
        }
        bool exists(const key_T& key) const
        {
            return binary_search(key)!=end_index;
        }
        bool set(const key_T& key, value_T& value)
        {
            if(exists(key))
            {
                add_or_set(Pair<key_T,value_T>(key, value));
                return true;
            }

            return false;
        }
        value_T& operator[](const key_T& key)
        {
            int index = binary_search(key);
            if(index!=end_index)
            {
                return data[index].value;
            }
            else
            {
                index = add_or_set(Pair<key_T,value_T>(key));
            }

            return data[index].value;
        }
        key_T get_key(unsigned int index) const
        {
            if(index>=0 && index < end_index)
            {
                return data[index].key;
            }
            return key_T();
        }
        bool remove(const key_T& key)
        {
            int index = binary_search(key);
            if(index!=end_index)
            {
                int retrieve_i;
                for(int i=index;i<end_index;i++)
                {
                    data[i]=data[i+1];
                }
                end_index--;
                return true;

            }

            return false;
        }
};