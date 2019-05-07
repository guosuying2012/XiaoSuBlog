#include <cppcms/serialization.h>
#include <cppcms/json.h>
#include <string>
#include <ctime>
#include <vector>

//用户信息结构体
struct user : public cppcms::serializable
{
    unsigned int nId;               //用户ID
    std::string strIp;              //注册IP
    std::string strName;            //用户名
    std::string strEmail;           //邮箱地址
    std::string strProfilePhoto;    //头像
    std::string strLevel;           //等级
    int nRights;                    //权限
    std::string strRegistrationTime;    //注册时间
    std::string strNikeName;        //昵称

    void clear()
    {
        nId = 0;
        strIp.clear();
        strName.clear();
        strEmail.clear();
        strProfilePhoto.clear();
        strLevel.clear();
        nRights = 0;
        strNikeName.clear();
    }

    void serialize(cppcms::archive &a)
    {
        a&nId&strIp&strName&strEmail&strProfilePhoto&strLevel&nRights&strRegistrationTime&strNikeName;
    }
};
using users = std::vector<user>;

//user json 方法
namespace cppcms 
{
    namespace json 
    {
        template<>
        struct traits<user> 
        {
            static user get(value const &v)
            {
                user out;
                if(v.type()!=is_object)
                {
                    throw bad_value_cast();
                }

                out.nId = v.get<unsigned int>("id");
                out.strIp = v.get<std::string>("ip");
                out.strName = v.get<std::string>("name");
                out.strEmail = v.get<std::string>("email");
                out.strProfilePhoto = v.get<std::string>("profile_photo");
                out.strLevel = v.get<std::string>("level");
                out.nRights = v.get<int>("rights");
                out.strRegistrationTime = v.get<std::string>("registration_time");
                out.strNikeName = v.get<std::string>("nikename");

                return out;
            }

            static void set(value &v,user const &in)
            {
                v.set("id", in.nId);
                v.set("ip", in.strIp);
                v.set("name", in.strName);
                v.set("email", in.strEmail);
                v.set("profile_photo", in.strProfilePhoto);
                v.set("level", in.strLevel);
                v.set("rights", in.nRights);
                v.set("registration_time", in.strRegistrationTime);
                v.set("nikename", in.strNikeName);
            }
        };
    } // json
} // cppcms


//博客信息结构体
struct article : public cppcms::serializable
{
    unsigned int nId;           //博客ID
    user m_user;                //用户
    std::string strTitle;       //标题
    std::string strContent;     //内容
    unsigned int nViews;        //浏览量
    unsigned int nCommentCount; //评论量
    std::string strTime;            //发布时间
    unsigned int nLikeCount;    //点赞量
    std::string strLastModified;    //最后后修改时间

    void clear()
    {
        nId = 0;
        m_user.clear();
        strTitle.clear();
        strContent.clear();
        nViews = 0;
        nCommentCount = 0;
        strTime.clear();
        nLikeCount = 0;
        strLastModified.clear();
    }

    void serialize(cppcms::archive &a)
    {
        a&nId&m_user&strTitle&strContent&nViews&nCommentCount&strTime&nLikeCount&strLastModified;
    }
};
using articles = std::vector<article>;

//article json 方法
namespace cppcms 
{
    namespace json 
    {
        template<>
        struct traits<article> 
        {
            static article get(value const &v)
            {
                article out;
                if(v.type()!=is_object)
                {
                    throw bad_value_cast();
                }

                out.nId = v.get<unsigned int>("id");
                out.m_user = v.get<user>("user");
                out.strTitle = v.get<std::string>("title");
                out.strContent = v.get<std::string>("content");
                out.nViews = v.get<unsigned int>("views");
                out.nCommentCount = v.get<unsigned int>("comment_count");
                out.strTime = v.get<std::string>("time");
                out.nLikeCount = v.get<unsigned int>("like_count");
                out.strLastModified = v.get<std::string>("last_modified");

                return out;
            }

            static void set(value &v,article const &in)
            {
                v.set("id", in.nId);
                v.set("user", in.m_user);
                v.set("title", in.strTitle);
                v.set("content", in.strContent);
                v.set("views", in.nViews);
                v.set("comment_count", in.nCommentCount);
                v.set("time", in.strTime);
                v.set("like_count", in.nLikeCount);
                v.set("last_modified", in.strLastModified);
            }
        };
    } // json
} // cppcms

//评论信息结构体
struct comment : public cppcms::serializable
{
    unsigned int nId;           //评论ID
    user m_user;                //评论用户
    article m_article;          //博客
    unsigned int nLikeCount;    //点赞量
    std::string strTime;        //发表时间
    std::string strContent;     //评论内容
    unsigned int nParentId;     //父评论ID

    void clear()
    {
        nId = 0;
        m_user.clear();
        m_article.clear();
        nLikeCount = 0;
        strTime.clear();
        strContent.clear();
        nParentId = 0;
    }

    void serialize(cppcms::archive &a)
    {
        a&nId&m_user&m_article&nLikeCount&strTime&strContent&nParentId;
    }
};
using comments = std::vector<comment>;

//comment json 方法
namespace cppcms 
{
    namespace json 
    {
        template<>
        struct traits<comment> 
        {
            static comment get(value const &v)
            {
                comment out;
                if(v.type()!=is_object)
                {
                    throw bad_value_cast();
                }

                out.nId = v.get<unsigned int>("id");
                out.m_user = v.get<user>("user");
                out.m_article = v.get<article>("article");
                out.nLikeCount = v.get<unsigned int>("like_count");
                out.strTime = v.get<std::string>("time");
                out.strContent = v.get<std::string>("content");
                out.nParentId = v.get<unsigned int>("parent");

                return out;
            }

            static void set(value &v,comment const &in)
            {
                v.set("id", in.nId);
                v.set("user", in.m_user);
                v.set("article", in.m_article);
                v.set("like_count", in.nLikeCount);
                v.set("time", in.strTime);
                v.set("content", in.strContent);
                v.set("parent", in.nParentId);
            }
        };
    } // json
} // cppcms

//博客设置选项结构体
struct option : public cppcms::serializable
{
    unsigned int nId;       //设置ID
    std::string strName;    //选项名称
    std::string strValue;   //值

    void clear()
    {
        nId = 0;
        strName.clear();
        strValue.clear();
    }

    void serialize(cppcms::archive &a)
    {
        a&nId&strName&strValue;
    }
};
using options = std::vector<option>;

//option json 方法
namespace cppcms 
{
    namespace json 
    {
        template<>
        struct traits<option> 
        {
            static option get(value const &v)
            {
                option out;
                if(v.type()!=is_object)
                {
                    throw bad_value_cast();
                }

                out.nId = v.get<unsigned int>("id");
                out.strName = v.get<std::string>("name");
                out.strValue = v.get<std::string>("value");

                return out;
            }

            static void set(value &v,option const &in)
            {
                v.set("id", in.nId);
                v.set("name", in.strName);
                v.set("value", in.strValue);
            }
        };
    } // json
} // cppcms

//分类信息结构体
struct sort : public cppcms::serializable
{
    unsigned int nId;       //分类ID
    unsigned int nParentId; //父ID
    std::string strName;    //分类名称
    int nRank;              //排序

    void clear()
    {
        nId = 0;
        nParentId = 0;
        strName.clear();
        nRank = 0;
    }

    void serialize(cppcms::archive &a)
    {
        a&nId&nParentId&strName&nRank;
    }
};
using sorts = std::vector<sort>;

//sort json 方法
namespace cppcms 
{
    namespace json 
    {
        template<>
        struct traits<sort> 
        {
            static sort get(value const &v)
            {
                sort out;
                if(v.type()!=is_object)
                {
                    throw bad_value_cast();
                }

                out.nId = v.get<unsigned int>("id");
                out.nParentId = v.get<unsigned int>("parent");
                out.strName = v.get<std::string>("name");
                out.nRank = v.get<int>("rank");

                return out;
            }

            static void set(value &v,sort const &in)
            {
                v.set("id", in.nId);
                v.set("parent", in.nParentId);
                v.set("name", in.strName);
                v.set("rank", in.nRank);
            }
        };
    } // json
} // cppcms

//博客分类结构体
struct SetArticleSort : public cppcms::serializable
{
    unsigned int nArticleId;        //博客ID
    unsigned int nSortId;           //分类ID
    
    void clear()
    {
        nArticleId = 0;
        nSortId = 0;
    }

    void serialize(cppcms::archive &a)
    {
        a&nArticleId&nSortId;
    }
};


