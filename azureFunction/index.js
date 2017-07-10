module.exports = function (context, eventHubMessages) {
    context.log(eventHubMessages);

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
            "value": eventHubMessages
            }
        ]
    };

    context.done(null, message);
};