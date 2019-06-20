var nNumber = 1;
var nType = 0;
var articles = [];
var nId = 0;
let url = new URL(window.location.href);

window.onload = function() 
{
    localStorage.removeItem("list");

    if (url.searchParams.has("author"))
    {
        nType = 1;
        nId = url.searchParams.get("author");
        sendRequest(nType, nId);
        return;
    }
    nType = 2;
    nId = url.searchParams.get("category");
    sendRequest(nType, nId);
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
            article += "<div class='media'><a href='?single=\""+obj.id+"\"'><img src='"+obj.image+"' alt='"+obj.title+"' /></a></div>";
        }
        article += "<h3><a href='single.html?single="+obj.id+"'>"+obj.title+"</a></h3> \
        <span><span>"+timetrans(obj.time)+"</span> \
        / by <a href='category.html?author="+obj.user.id+"'><span>"+ obj.user.displayname +"</span></a> \
        / in: <span><a href='category.html?category="+obj.sort.id+"'>"+obj.sort.name+"</a></span> \
        / <a href='single.html?single="+obj.id+"#comments'><span>"+obj.comment_count+"</span> Comments</a></span> \
        </header> <div class='editor-styles' style='text-indent:2em'>"+obj.describe+"......</div> <footer> <div> \
        <a href='single.html?single="+obj.id+"''>Continue Reading...</a> \
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
    sendRequest(nType, nId);
}

function sendRequest(type, nId) 
{
    var api = "";
    switch (parseInt(type))
    {
    case 1:
        api = author_articles("/"+nId+"/"+ nNumber);
        break;
    case 2:
        api = sort_articles("/"+nId+"/"+ nNumber);
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
        case 1:
            $("#sub_title").html("Author");
            $("#title").html(resArticles.data[0].user.displayname);
            break;
        case 2:
            $("#sub_title").html("Category");
            sort_byid("/"+nId).
            then(function(sort) {
                $("#title").html(sort.data.name);
                document.title = sort.data.name + " - 小蘇-XiaoSU";
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


