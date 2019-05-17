window.onload = function() 
{
    localStorage.clear();
};

//文章列表
function article_callback(response) 
{
    for (var i = 0; i <= response.data.length - 1; ++i) 
    {
        var obj = response.data[i];
        var article = "";
        article = "<article class='post'> <header>";
        if (obj.image != undefined)
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

var nNumber = 1;
function loadmore() 
{
    ++nNumber;
    article_list("/" + nNumber)
    .then(response=>
    {
        if (response.data != "null") 
        {
            article_callback(response);
            return;
        }
        spopAlert(response.error, "info", "bottom-right");
    })
    .catch(error=>
    {
        $("#preloader").fadeOut(500);
        spopAlert(error.error, "error", "bottom-right");
        return;
    });
}

//接口并发
axios.all([article_list("/1")])
.then(axios.spread(function (resArticles) 
{
    if (resArticles.data != "null") 
    {
        article_callback(resArticles);
    }

    $("#content").append("<nav id='post-nav'><a href='javascript:void(0)' onclick='loadmore()'>Older Posts »</a></nav>")
}))
.catch(err=>
{
    spopAlert(err.error, "error", "bottom-right");
    return;
});

