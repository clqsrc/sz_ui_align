//{$I ..\..\source\compiler.inc}
{$I 3rd\PasLibVlc_3.0.7\PasLibVlc\source\compiler.inc}
//..\VideoPlay_delphi_mp3_2023\3rd\PasLibVlc_3.0.7\PasLibVlc\Delphi7\DemoPasLibVlc

unit vlc2023form;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls;

type
  TForm1 = class(TForm)
    OpenDialog1: TOpenDialog;
    ScrollBox1: TScrollBox;
    Panel1: TPanel;
    Button1: TButton;
    MrlEdit: TEdit;
    Button2: TButton;
    procedure FormCreate(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
    //PasLibVlcPlayer1: TPasLibVlcPlayer;
  end;

var
  Form1: TForm1;

implementation

{$R *.dfm}

uses
  //PasLibVlcUnit,
  PasLibVlcUnit, PasLibVlcClassUnit, PasLibVlcPlayerUnit;

var
  PasLibVlcPlayer1: TPasLibVlcPlayer;


procedure TForm1.FormCreate(Sender: TObject);
begin

  //好像要先设置 vlc 的路径
  //TPasLibVlc.
  //----

  PasLibVlcPlayer1 := TPasLibVlcPlayer.Create(Self);
  //PasLibVlcPlayer1.Parent := Self;
  PasLibVlcPlayer1.Parent := Self.ScrollBox1;
  PasLibVlcPlayer1.VLC.Path := 'F:\new\VideoLAN\VLC';  //vlc 的安装路径，一定要正确
  PasLibVlcPlayer1.Show;

  PasLibVlcPlayer1.Width := 1920 * 2;
  PasLibVlcPlayer1.Height := 1080 * 2;

  PasLibVlcPlayer1.Width := 1920 * 4;
  PasLibVlcPlayer1.Height := 1080 * 4;

  //PasLibVlcPlayer1.Play('');
  //PasLibVlcPlayer1.Play(MrlEdit.Text);

end;

procedure TForm1.Button1Click(Sender: TObject);
begin
  PasLibVlcPlayer1.Play(MrlEdit.Text);


//注意 delphi 7 是要用 32 位版本的 vlc 的，如果不对，下面这个位置会报错
(*

function TPasLibVlc.GetHandle() : libvlc_instance_t_ptr;
begin
  Result := NIL;
  if (FHandle = NIL) then
  begin
    if (FPath <> '') then
    begin
      libvlc_dynamic_dll_init_with_path(FPath);
      if (libvlc_dynamic_dll_error <> '') then libvlc_dynamic_dll_init();

//----------------------------------------------------------------
TPasLibVlcPlayer.PlayNormal
TPasLibVlcPlayer.GetPlayerHandle()
function TPasLibVlc.GetHandle() : libvlc_instance_t_ptr;
procedure libvlc_dynamic_dll_init();      

*)

end;

procedure TForm1.Button2Click(Sender: TObject);
begin
  if OpenDialog1.Execute then
  MrlEdit.Text := OpenDialog1.FileName;
end;

end.
