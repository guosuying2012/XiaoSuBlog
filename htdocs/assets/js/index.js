window.onload = function() 
{
    localStorage.clear();
};

//文章列表
article_list()
.then(response=>
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
                    <span><span>"+obj.time+"</span> \
                    / by <a href='javascript:void(0)' onclick='pageJump(LinkTypeEnum.AUTHOR, \""+obj.id+"\")'><span>"+ obj.author +"</span></a> \
                    / in: <span><a href='javascript:void(0)' onclick='pageJump(LinkTypeEnum.TRAVEL, \""+obj.id+"\")'>"+obj.travel+"</a></span> \
                    / <a href='javascript:void(0)' onclick='pageJump(LinkTypeEnum.COMMENTS, \""+obj.id+"\")'><span>"+obj.comments+"</span> Comments</a></span> \
                    </header> \
                    <div class='editor-styles'>"+obj.content+"</div> \
                    <footer> \
                    <div> \
                    <a href='javascript:void(0)' onclick='pageJump(LinkTypeEnum.TEXT, \""+obj.uuid+"\")'>Continue Reading...</a> \
                    </div><hr> \
                    </footer> \
                    </article>";
        $("#list").append(article);
    }
})
.catch(error=>
{
    console.log(error);
});

//滚动图片
slider_images()
.then(response=>
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
        slider_imgs[i].setAttribute("src", response.data[i]["src"]);
        slider_imgs[i].setAttribute("alt", response.data[i]["description"]);
        description[i].innerHTML = response.data[i]["description"];
    }
})
.catch(error=>
{
    console.log(error);
});

//导航栏
navigation_bar()
.then(response=>
{
    //思路参考：https://blog.csdn.net/Mr_JavaScript/article/details/82817177
    let cloneData = JSON.parse(JSON.stringify(response.data));
    var tree = cloneData.filter(father=>{
        let branchArr = cloneData.filter(child=>father.id == child.parentId);
        branchArr.length>0 ? father.children=branchArr : '';
        return father.parentId==0;
    });
    navigation(tree, "#menu");
})
.catch(error=>
{
    console.log(error);
});

//数据库测试接口
test_database(0)
.then(response=>{
    console.log(response);
})
.catch(error=>{
    console.log(error);
});

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

$.getScript("assets/js/plugins.js",function()
{
    $.getScript("assets/js/scripts.js");
});



