module.exports = function (context, eventHubMessage) {
    context.log(eventHubMessage);

    var message = {
        // could use temporary settings in the comment below and uncomment it
        /*"personalizations": [
            {
            "to": [
                {
                "email": "<temporary toEmail>"
                }
            ],
            "subject": "<temporary subject>"
            }
        ],
        "from": {
            "email": "<temporary fromEmail>"
        },*/
        "content": [
            {
            "type": "text/plain",
            "value": eventHubMessage
            }
        ]
    };

    context.done(null, message);
};