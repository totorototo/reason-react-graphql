module Styles = {
  open Css;
  let container = _ =>
    style([
      margin(px(5)),
      padding(px(5)),
      backgroundColor(Theme.secondaryColor),
      color(white),
      borderRadius(px(3)),
      width(px(300)),
    ]);
};

let component = ReasonReact.statelessComponent("ChannelBubble");

let make = (~channel, _children) => {
  ...component,
  render: _ =>
    <div className={Styles.container(channel##name === "my-id")}>
      {channel##name |> ReasonReact.string}
      {
        channel##messages
        |> Js.Array.map(message => message##text |> ReasonReact.string)
        |> ReasonReact.array
      }
    </div>,
};