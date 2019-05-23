
window.onload = function() 
{
    var nId = localStorage.getItem("id");
    getArticle(nId);
}

function getArticle(articleId) 
{
    article("/" + articleId)
    .then(article=> 
    {
        getUser(article.data.user.id);
        getComment(articleId);

        $("#header").append("<h3><a>"+article.data.title+"</a></h3><span>"+timetrans(article.data.time)+" / "+ article.data.comment_count +" Comments</a></span>");
        if (article.data.image != "") 
        {
            $("#header").append("<div class='media'><img src='"+ article.data.image +"' alt=''></div>");
        }
        $("#content").append(article.data.content);
        $("#comment").html(article.data.comment_count + " Comments");
    })
    .catch(err=>
    {
        spopAlert(err.error, "error", "bottom-right");
        return;
    });
}

function getUser(userId) 
{
    user("/" + userId)
    .then(user=> 
    {
        $("#post-author").append("<h4><a href='javascript:void(0)' onclick='pageJump(LinkTypeEnum.AUTHOR, \""+user.data.id+"\")'>"+user.data.nikename+"</a></h4>");
        $("#post-author").append("<p>"+ user.data.signature +"</p>");
        if (user.data.profile_photo != "") 
        {
            $("#post-author").append("<img src='"+user.data.profile_photo+"' alt=''>");
        }
        else
        {
            $("#post-author").append("<img src='assets/img/avatar.png' alt=''>");
        }
    })
    .catch(err=>
    {
        spopAlert(err.error, "error", "bottom-right");
        return;
    });
}

function getComment(articleId) 
{
    comment("/" + articleId)
    .then(comment=> 
    {
        console.log(comment);
    })
    .catch(err=>
    {
        spopAlert(err.error, "error", "bottom-right");
        return;
    });
}
