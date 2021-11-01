module.exports = function (RED) {
    function SPC(config) {
        RED.nodes.createNode(this, config);
        var node = this;
        //
        node.lastValue = 0.0;
        node.trendCount = 0;
        node.meanCrowdingCount = 0;
        node.limitCount = 0;
        node.trendUp = true;
        node.trendDown = true;
        //
        // Do the SPC ALarms
        //
        node.upperLimit = parseFloat(config.upperLimit);
        node.lowerLimit = parseFloat(config.lowerLimit);
        node.limitTriggerCount = parseInt(config.limitTriggerCount);
        //
        node.meanCrowdingLimit = parseInt(config.meanCrowdingLimit);
        node.trendCountLimit = parseInt(config.trendCountLimit);
        //
        node.upperLimitEnable = config.upperLimitEnable;
        node.lowerLimitEnable = config.lowerLimitEnable;
        //
        node.meanCrowdingEnable = config.meanCrowdingEnable;
        node.trendEnable = config.trendEnable;
        //
        //
        node.limitonvalue = RED.util.evaluateNodeProperty(config.limitonvalue, config.limitonvalueType, node); // translate to correct type
        node.limitoffvalue = RED.util.evaluateNodeProperty(config.limitoffvalue, config.limitoffvalueType, node); // translate to correct type
        node.meanCrowdingonvalue = RED.util.evaluateNodeProperty(config.meanCrowdingonvalue, config.meanCrowdingonvalueType, node); // translate to correct type
        node.meanCrowdingoffvalue = RED.util.evaluateNodeProperty(config.meandCrowdingoffvalue, config.meanCrowdingoffvalueType, node); // translate to correct type
        node.trendonvalue = RED.util.evaluateNodeProperty(config.trendonvalue, config.trendonvalueType, node); // translate to correct type
        node.trendoffvalue = RED.util.evaluateNodeProperty(config.trendoffvalue, config.trendoffvalueType, node); // translate to correct type
        //
        //    
        node.on('input', function (msg) {
            var v = 0.0;

            if (typeof (msg.payload) == 'number') {
                v = msg.payload;
            } else if (typeof (msg.payload) == 'string') {
                v = parseFloat(msg.payload);
            }
            //
            var limitTrigger = false;
            var trendTrigger = false;
            var meanCrowdingTrigger = false;
            //
            if ((this.upperLimitEnable && (v > this.upperLimit)) || (this.lowerLimitEnable && (v < this.lowerLimit))) {
                this.limitCount++;
                if (this.limitCount > this.limitTriggerCount) {
                    limitTrigger = true;
                }
                this.meanCrowdingCount = 0;
            }
            else if (this.meanCrowdingEnable) {
                // Check the mean crowding 
                this.meanCrowdingCount++;
                this.limitCount = 0;
                if (this.meanCrowdingCount > this.meanCrowdingLimit) {
                    meanCrowdingTrigger = true;
                }
            }

            // detect trending
            if (this.trendEnable) {
                if (v > this.lastValue) {
                    if (this.trendDown) this.trendCount = 0;
                    if (this.trendUp) this.trendCount++;
                    this.trendDown = false;
                    this.trendUp = true;
                }
                else if (v < this.lastValue) {

                    if (this.trendUp) this.trendCount = 0;
                    if (this.trendDown) this.trendCount++;
                    this.trendDown = true;
                    this.trendUp = false;
                }
                else {
                    this.trendCount = 0;
                    this.trendDown = false;
                    this.trendUp = false;
                }
                this.lastValue = v;
                if (this.trendCount > this.trendCountLimit) trendTrigger = true;
            }

            msg.payload = v;
            //  


            // we have four outputs  value, ok, action , alert
            var triggered = limitTrigger || trendTrigger || meanCrowdingTrigger;
            var state = 'ok';

            //
            // set flags - the messages may drive outputs - so we need on and off for each possible status
            //
            var limitMsg = { payload: limitTrigger ? this.limitonvalue : this.limitoffvalue, topic: msg.topic, value: v, state: limitTrigger ? 'action' : 'ok' };
            var trendMsg = { payload: trendTrigger ? this.trendonvalue : this.trendoffvalue, topic: msg.topic, value: v, state: trendTrigger ? 'action' : 'ok' };
            var meanCrowdingMsg = { payload: meanCrowdingTrigger ? this.meanCrowdingonvalue : this.meanCrowdingoffvalue, topic: msg.topic, value: v, state: meanCrowdingTrigger ? 'action' : 'ok' };
            //
            if (triggered) {
                this.status({ fill: "red", shape: "dot", text: "SPC" });
                state = 'action';
            }
            else {
                if (this.limitCount > 0) {
                    this.status({ fill: "yellow", shape: "dot", text: "Alert" });
                    state = 'alert';
                }
                else {
                    this.status({ fill: "green", shape: "dot", text: "OK" });
                }
            }
            //
            msg.state = state; // add the value's alarm state to the value message - use this to connect to alarm groups
            node.send([msg, limitMsg, trendMsg, meanCrowdingMsg]);
        });
    }
    RED.nodes.registerType("spc", SPC);
}
