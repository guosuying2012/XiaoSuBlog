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
        / by <a href='javascript:void(0)' onclick='pageJump(LinkTypeEnum.AUTHOR, \""+obj.id+"\")'><span>"+ obj.user.name +"</span></a> \
        / in: <span><a href='javascript:void(0)' onclick='pageJump(LinkTypeEnum.TRAVEL, \""+obj.id+"\")'>"+obj.sort.name+"</a></span> \
        / <a href='javascript:void(0)' onclick='pageJump(LinkTypeEnum.COMMENTS, \""+obj.id+"\")'><span>"+obj.comment_count+"</span> Comments</a></span> \
        </header> <div class='editor-styles'>"+obj.describe+"</div> <footer> <div> \
        <a href='javascript:void(0)' onclick='pageJump(LinkTypeEnum.TEXT, \""+obj.id+"\")'>Continue Reading...</a> \
        </div><hr> </footer> </article>";
        $("#list").append(article);
    }
}

//滚动图片
function slider_callback(response) 
{
    for (var i = response.data.length - 1; i >= 0; i--) 
    {
        var slider = $("<div><a href='#'><img class='slider_image'></a> \
                    <span class='description'></span> \
                    </div>");
        $("#featured").append(slider);
    }

    var slider_imgs = document.getElementsByClassName("slider_image");
    var description = document.getElementsByClassName("description");
    for (var i = slider_imgs.length - 1; i >= 0; i--) 
    {
        slider_imgs[i].setAttribute("src", response.data[i]["path"]);
        slider_imgs[i].setAttribute("alt", response.data[i]["description"]);
        description[i].innerHTML = response.data[i]["description"];
    }
}

//导航栏
function navigation_callback(response) 
{
    //思路参考：https://blog.csdn.net/Mr_JavaScript/article/details/82817177
    let cloneData = JSON.parse(JSON.stringify(response.data));
    var tree = cloneData.filter(father=>{
        let branchArr = cloneData.filter(child=>father.id == child.parentId);
        branchArr.length>0 ? father.children=branchArr : '';
        return father.parentId==0;
    });
    navigation(tree, "#menu");
}

//思路参考：https://blog.csdn.net/cc_fys/article/details/81284638
function navigation(tree, parentElement) 
{
    for (var i = 0; i <= tree.length - 1; ++i)
    {
        var obj = tree[i];
        var element = document.createElement('li');
        if (obj.children != undefined)
        {
            element.innerHTML = "<a href='javascript:void(0)' onclick='pageJump(LinkTypeEnum.TRAVEL, \""+obj.id+"\")'>"+obj.name+"</a>";
            var nextpar = document.createElement('ul');
            nextpar.classList.add("sub-menu");
            element.appendChild(nextpar);
            navigation(obj.children ,nextpar);
        }
        else
        {
            element.innerHTML = "<a href='javascript:void(0)' onclick='pageJump(LinkTypeEnum.TRAVEL, \""+obj.id+"\")'>"+obj.name+"</a>";
        }
        $(parentElement).append(element);
    }
}

//接口并发
axios.all([article_list("/1"), slider_images(), navigation_bar()])
.then(axios.spread(function (resArticles, resSlider, resNavigation) 
{
    if (resArticles.data === "null" 
        || resSlider.data === "null" 
        || resNavigation.data === "null") 
    {
        return;
    }

    article_callback(resArticles);
    slider_callback(resSlider);
    navigation_callback(resNavigation);

    $.getScript("assets/js/plugins.js",function()
    {
        $.getScript("assets/js/scripts.js");
        $("#preloader").fadeOut(500);
    });
}))
.catch(err=>
{
    console.log(err);
    return;
});

