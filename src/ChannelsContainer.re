[@bs.module] external gql: ReasonApolloTypes.gql = "graphql-tag";

module Styles = {
  open Css;
  let container =
    style([
      margin(px(20)),
      padding(px(5)),
      boxShadow(~x=px(2), ~y=px(2), grey),
    ]);
};

module GetChannels = [%graphql
  {|
  {
    channels {
      id
      name
      messages{
          id
          text
      }
    }
  }
|}
];

module ChannelAdded = [%graphql
  {|
subscription {
  channelAdded {
    id
    name
  }
}
|}
];

let channelAdded = ChannelAdded.make();
let channelAddedASTQuery = gql(. channelAdded##query);

module GetChannelsQuery = ReasonApollo.CreateQuery(GetChannels);

let component = ReasonReact.statelessComponent("Messages");

let make = _children => {
  ...component,
  render: _self =>
    <div className=Styles.container>
      <GetChannelsQuery>
        ...{
             ({result, subscribeToMore}) =>
               switch (result) {
               | Loading => <div> {"Loading" |> ReasonReact.string} </div>
               | Error(_e) => <div> {"Error" |> ReasonReact.string} </div>
               | Data(response) =>
                 <Channels
                   onLoad=(
                     () => {
                       let _unsub =
                         subscribeToMore(
                           ~document=channelAddedASTQuery,
                           ~updateQuery=
                             (prev, next) => {
                               let addNewChannelJS = [%bs.raw
                                 {|
                          function(prev, next) {
                            if(!next.subscriptionData.data || !next.subscriptionData.data.channelAdded)
                              return prev;
                            return Object.assign({}, prev, {
                              channels: prev.channels.concat(next.subscriptionData.data.channelAdded)
                            });
                          }
                      |}
                               ];
                               addNewChannelJS(prev, next);
                             },
                           (),
                         );
                       ();
                     }
                   )
                   channels=response##channels
                 />
               }
           }
      </GetChannelsQuery>
    </div>,
};