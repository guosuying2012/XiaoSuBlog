

axios.all([])
.then(axios.spread(function () 
{
    
}))
.catch(err=>
{
    spopAlert(err.error, "error", "bottom-right");
    return;
});