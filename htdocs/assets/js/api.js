const article_list = p=>get("index/getArticles", p);
const slider_images = p=>get("settings/getSliderImages", p);
const navigation_bar = p=>get("settings/getNavigations", p);
const website_options = p=>get("settings/getWebsiteOptions", p);
const author_articles = p=>get("category/getAuthorArticles", p);
const sort_articles = p=>get("category/getSortArticles", p);
const sort_byid = p=>get("category/getSortById", p);
const user = p=>get("user/getUserById", p);
const article = p=>get("article/getArticleById", p);
const comment = p=>get("comment/getCommentByArticleId", p);
const comment_byid = p=>get("comment/getCommentById", p);
const post_comment = p=>post("comment/postComment", p);


















