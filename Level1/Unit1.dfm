object Form1: TForm1
  Left = 198
  Top = 123
  Width = 760
  Height = 516
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object PG_Ctl: TPageControl
    Left = 0
    Top = 0
    Width = 744
    Height = 381
    ActivePage = ts_Lvl1
    Align = alTop
    TabIndex = 0
    TabOrder = 0
    OnChange = PG_CtlChange
    OnChanging = PG_CtlChanging
    object ts_Lvl1: TTabSheet
      Caption = 'Level_1'
      object Label1: TLabel
        Left = 4
        Top = 8
        Width = 212
        Height = 13
        Caption = 'SOURCE: (UBoot based  firmware file (*.img))'
      end
      object Label2: TLabel
        Left = 4
        Top = 80
        Width = 458
        Height = 13
        Caption = 
          'DESTINATION: (Select the ROOT folder, individual files will be c' +
          'reated in the "Level1" subfolder)'
      end
      object Ed_ImgFile: TEdit
        Left = 8
        Top = 28
        Width = 688
        Height = 21
        TabOrder = 0
        Text = 'D:\MINIX\ZZZ\S912-v008-20180209.img'
      end
      object Btn_FWImgSel: TButton
        Left = 700
        Top = 24
        Width = 25
        Height = 25
        Caption = '...'
        TabOrder = 1
        OnClick = Btn_FWImgSelClick
      end
      object Btn_UnpLvl1: TButton
        Left = 56
        Top = 320
        Width = 601
        Height = 25
        Caption = 
          'Unpack to level 1 (split to separate files and place it'#39's to the' +
          ' destination subfolder "LEVEL1") '
        TabOrder = 2
        OnClick = Btn_UnpLvl1Click
      end
      object ChkBx_SrcCrc32: TCheckBox
        Left = 8
        Top = 48
        Width = 757
        Height = 17
        Caption = 
          'Do check control sum of the source file (slight slowdown if enab' +
          'led, but disabling this option is strictly not recommended)'
        Checked = True
        State = cbChecked
        TabOrder = 3
      end
      object Ed_TargetDir: TEdit
        Left = 8
        Top = 100
        Width = 688
        Height = 21
        TabOrder = 4
        Text = 'D:\MINIX\FWWORK'
        OnChange = Ed_TargetDirChange
      end
      object Btn_SelDest: TButton
        Left = 700
        Top = 97
        Width = 25
        Height = 25
        Caption = '...'
        TabOrder = 5
        OnClick = Btn_SelDestClick
      end
      object ChkBx_CleanWarn: TCheckBox
        Left = 8
        Top = 120
        Width = 685
        Height = 17
        Caption = 
          'Prompt before clean this folder (Legend: Black - Empty,  Blue - ' +
          'Is NOT Empty, Red - Does not exist, will be created)'
        TabOrder = 6
      end
    end
    object ts_Lvl2: TTabSheet
      Caption = 'Level_2'
      ImageIndex = 1
      object LsBx_FilesL1: TListBox
        Left = 0
        Top = 4
        Width = 245
        Height = 345
        ItemHeight = 13
        TabOrder = 0
        OnClick = LsBx_FilesL1Click
      end
    end
    object ts_Log: TTabSheet
      Caption = 'Log'
      ImageIndex = 2
      object LsBxLog: TListBox
        Left = 0
        Top = 0
        Width = 736
        Height = 361
        Align = alTop
        ItemHeight = 13
        TabOrder = 0
      end
    end
  end
  object Button3: TButton
    Left = 200
    Top = 420
    Width = 75
    Height = 25
    Caption = 'Button3'
    TabOrder = 1
    OnClick = Button3Click
  end
  object Button2: TButton
    Left = 96
    Top = 420
    Width = 75
    Height = 25
    Caption = 'Button2'
    TabOrder = 2
    OnClick = Button2Click
  end
  object OpenDialog: TOpenDialog
    OnShow = OpenDialogShow
    Filter = 'Firmware files *.img|*.img|All files *.*|*.*'
    FilterIndex = 0
    Left = 700
    Top = 12
  end
end
