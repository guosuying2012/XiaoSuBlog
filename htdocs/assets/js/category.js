var nId = 0;
var nType = 0;
var nNumber = 1;

window.onload = function() 
{
    nId = localStorage.getItem("id");
    nType = localStorage.getItem("type");
    sendRequest();
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
        / by <a href='javascript:void(0)' onclick='pageJump(LinkTypeEnum.AUTHOR, \""+obj.user.id+"\")'><span>"+ obj.user.name +"</span></a> \
        / in: <span><a href='javascript:void(0)' onclick='pageJump(LinkTypeEnum.TRAVEL, \""+obj.sort.id+"\")'>"+obj.sort.name+"</a></span> \
        / <a href='javascript:void(0)' onclick='pageJump(LinkTypeEnum.COMMENTS, \""+obj.id+"\")'><span>"+obj.comment_count+"</span> Comments</a></span> \
        </header> <div class='editor-styles' style='text-indent:2em'>"+obj.describe+"......</div> <footer> <div> \
        <a href='javascript:void(0)' onclick='pageJump(LinkTypeEnum.TEXT, \""+obj.id+"\")'>Continue Reading...</a> \
        </div><hr> </footer> </article>";
        $("#list").append(article);
    }
}

function loadmore() 
{
    ++nNumber;
    sendRequest();
}

function sendRequest() 
{
    var api = "";
    switch (parseInt(nType))
    {
    case LinkTypeEnum.AUTHOR:
        api = author_articles("/"+nId+"/"+nNumber);
        break;
    case LinkTypeEnum.TRAVEL:
        api = sort_articles("/"+nId+"/"+nNumber);
        break;
    }

    axios.all([api])
    .then(axios.spread(function(resArticles) 
    {
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

        switch (parseInt(nType))
        {
        case LinkTypeEnum.AUTHOR:
            $("#sub_title").html("Author");
            $("#title").html(resArticles.data[0].user.name);
            break;
        case LinkTypeEnum.TRAVEL:
            $("#sub_title").html("Category");
            $("#title").html(resArticles.data[0].sort.name);
            break;
        }

    }))
    .catch(err=>
    {
        spopAlert(err.error, "error", "bottom-right");
        return;
    });
}


