let component = ReasonReact.statelessComponent("Channels");

let make = (~channels, ~onLoad, _children) => {
  ...component,
  didMount: _self => onLoad(),
  render: _self =>
    <div>
      {
        channels
        |> Js.Array.map(channel => <ChannelBubble channel key=channel##id />)
        |> ReasonReact.array
      }
    </div>,
};