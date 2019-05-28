var nType = 0;
var nNumber = 1;
var articles = [];

window.onload = function() 
{
    nType = localStorage.getItem("type");
    localStorage.removeItem("list");
    sendRequest(nType);
}

//文章列表
function article_callback(response) 
{
    for (var i = 0; i <= response.data.length - 1; ++i) 
    {
        var obj = response.data[i];
        var article = "";
        article = "<article class='post'> <header>";
        if (obj.image != "")
        {
            article += "<div class='media'><a href='javascript:void(0)' onclick='pageJump(LinkTypeEnum.TEXT, \""+obj.id+"\")'><img src='"+obj.image+"' alt='"+obj.title+"' /></a></div>";
        }
        article += "<h3><a href='javascript:void(0)' onclick='pageJump(LinkTypeEnum.TEXT, \""+obj.id+"\")'>"+obj.title+"</a></h3> \
        <span><span>"+timetrans(obj.time)+"</span> \
        / by <a href='javascript:void(0)' onclick='pageJump(LinkTypeEnum.AUTHOR, \""+obj.user.id+"\")'><span>"+ obj.user.displayname +"</span></a> \
        / in: <span><a href='javascript:void(0)' onclick='pageJump(LinkTypeEnum.TRAVEL, \""+obj.sort.id+"\")'>"+obj.sort.name+"</a></span> \
        / <a href='javascript:void(0)' onclick='pageJump(LinkTypeEnum.COMMENTS, \""+obj.id+"\")'><span>"+obj.comment_count+"</span> Comments</a></span> \
        </header> <div class='editor-styles' style='text-indent:2em'>"+obj.describe+"......</div> <footer> <div> \
        <a href='javascript:void(0)' onclick='pageJump(LinkTypeEnum.TEXT, \""+obj.id+"\")'>Continue Reading...</a> \
        </div><hr> </footer> </article>";
        $("#list").append(article);

        var data = {};
        data["id"] = obj.id;
        data["title"] = obj.title;
        articles.push(data);
    }
    localStorage.setItem("list", JSON.stringify(articles));
}

function loadmore() 
{
    ++nNumber;
    sendRequest(nType);
}

function sendRequest(type) 
{
    var api = "";
    switch (parseInt(type))
    {
    case LinkTypeEnum.AUTHOR:
        var nId = localStorage.getItem("author_id");
        api = author_articles("/"+nId+"/"+nNumber);
        localStorage.setItem("LastType", type);
        break;
    case LinkTypeEnum.TRAVEL:
        var nId = localStorage.getItem("travel_id");
        api = sort_articles("/"+nId+"/"+nNumber);
        localStorage.setItem("LastType", type);
        break;
    default:
        var lastType = localStorage.getItem("LastType");
        localStorage.setItem("type", lastType);
        sendRequest(lastType);
        localStorage.removeItem("LastType");
        break;
    }

    if (api === "") 
    {
        return;
    }

    axios.all([api])
    .then(axios.spread(function(resArticles) 
    {
        switch (parseInt(nType))
        {
        case LinkTypeEnum.AUTHOR:
            $("#sub_title").html("Author");
            $("#title").html(resArticles.data[0].user.displayname);
            break;
        case LinkTypeEnum.TRAVEL:
            $("#sub_title").html("Category");
            var nId = localStorage.getItem("travel_id");
            sort_byid("/"+nId).
            then(function(sort) {
                $("#title").html(sort.data.name);
            })
            .catch(function(error) {
                spopAlert(error.error, "info", "bottom-right");
            })
            break;
        }
        
        if (resArticles.data != "null") 
        {
            article_callback(resArticles);
        }
        else
        {
            spopAlert(resArticles.error, "error", "bottom-right");
            return;
        }

        var post = document.getElementById("post-nav");
        if (post === null) 
        {
            $("#content").append("<nav id='post-nav'><a href='javascript:void(0)' onclick='loadmore()'>Older Posts »</a></nav>");
        }
    }))
    .catch(err=>
    {
        spopAlert(err.error, "error", "bottom-right");
        return;
    });
}


